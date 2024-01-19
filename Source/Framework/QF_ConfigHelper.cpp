#include "QF_ConfigHelper.h"

#include <iostream>
#include <limits.h>
#include <stdlib.h>

#include "Libraries/cJSON/QC_cJSON.h"

#include "QC_Reflection.h"

#include "QC_Memory.h"
#include "QF_Defines.h"
#include "QF_Log.h"
#include "QF_Input.h"

namespace QwerkE {

    ConfigData ConfigHelper::m_ConfigData = ConfigData();
    UserData ConfigHelper::m_UserData = UserData();

    namespace Serialization
    {
#if _WIN32
        typedef INT32 PlatformPointer; // #TODO Handle pointer size on different systems better
#elif _WIN64
        typedef INT64 PlatformPointer;
#pragma warning "Implement 64 bit support!"
#endif

        void DeserializeObject(const cJSON* objJSON, const Reflection::ClassInfo* objClassInfo, void* obj);

        template <class T>
        void DeserializeObject(const cJSON* objJSON, T& obj)
        {
            DeserializeObject(objJSON, Reflection::GetClass<T>(), (void*)&obj);
        }

        template <class T>
        static void DeserializeObjectByTypeKey(const cJSON* jsonObj, T& obj)
        {
            auto typeInfo = Reflection::GetTypeInfo<T>();
            DeserializeObject<T>(GetItemFromRootByKey(jsonObj, typeInfo->stringName.c_str()), obj);
        }

        void DeserializeObject(const cJSON* objJSON, const Reflection::ClassInfo* objClassInfo, void* obj)
        {
            if (!objJSON)
            {
                LOG_ERROR("{0} {1} is null!", __FUNCTION__, VARNAME_TO_STR(objJSON));
                return;
            }

            if (!objClassInfo)
            {
                LOG_ERROR("{0} Class info serialization error!", __FUNCTION__);
                return;
            }

            if (!obj)
            {
                LOG_ERROR("{0} object reference {1} is null!", __FUNCTION__, VARNAME_TO_STR(obj));
                return;
            }

            // #TODO O(n^2) complexity is high. Iterate to improve performance
            // #TODO Also consider iterating over the class fields as priority vs cJSON data, as some old data/extra may still exist.
            const std::vector<cJSON*> arr = GetAllItemsFromArray(objJSON);
            for (size_t i = 0; i < arr.size(); i++)
            {
                const cJSON* arrItem = arr[i];

                for (size_t i = 0; i < objClassInfo->fields.size(); i++)
                {
                    const Reflection::Field& field = objClassInfo->fields[i];

                    if (!field.type)
                        break; // #TODO Review #NOTE Break early if fields become null (and future values are null) to save iterations

                    if (strcmp(field.name.c_str(), arrItem->string) != 0)
                        continue;

                    switch (arrItem->type)
                    {
                    case cJSON_String:
                        {
                            // #TODO Worry about the null terminating character '\0'
                            // #TODO Handle case where target/strPointerAddress is null or empty

                            if (!arrItem->valuestring)
                            {
                                LOG_ERROR("{0} Null cJSON string!", __FUNCTION__);
                                continue;
                            }

                            switch (field.type->enumType) // What kind of string for field
                            {
                                case TypeName::String:
                                {
                                    size_t smallerSize = strlen(arrItem->valuestring);

                                    std::string* fieldAddress = reinterpret_cast<std::string*>((char*)obj + field.offset); // #TODO Review string* increment size/stride
                                    if (smallerSize > fieldAddress->size())
                                    {
                                        smallerSize = field.type->size;
                                    }

                                    if (smallerSize > 0)
                                    {
                                        const void* sourceAddr = (const void*)arrItem->valuestring;
                                        memcpy((void*)fieldAddress->data(), sourceAddr, smallerSize);
                                    }
                                }
                                break;

                            case TypeName::Char:
                                {
                                    arrItem->valuestring;
                                    void* fieldAddress = (char*)obj + field.offset;
                                    memcpy(fieldAddress, arrItem->valuestring, sizeof(void*));
                                }
                                break;

                            case TypeName::CharPtr:
                            case TypeName::ConstCharPtr:
                                {
                                    // #TODO Validate field.type->size
                                    PlatformPointer* writeAddress = (PlatformPointer*)((char*)obj + field.offset);

                                    // if (writeAddress) // #TODO How to know if casted memory address is valid?
                                    {
                                        // #TODO How to know allocation function new vs new [] vs malloc?
                                        // delete[] writeAddress;
                                    }

                                    *writeAddress = (PlatformPointer)DeepCopyString(arrItem->valuestring);
                                }
                                break;

                            case TypeName::eKeys: // #TODO Consider using a number value instead of a string
                                {
                                    const char result = arrItem->valuestring[0];
                                    if (field.type->size >= sizeof(const char))
                                    {
                                        void* fieldAddress = (char*)obj + field.offset;
                                        memcpy(fieldAddress, &result, sizeof(const char));
                                    }
                                    else
                                    {
                                        LOG_WARN("{0} Unable to deserialize value for cJSON value {1} and field type {2}", __FUNCTION__, arrItem->string, field.type->stringName);
                                    }
                                }
                                break;

                            default:
                                LOG_ERROR("{0} Unsupported field type {1}({2}) for deserialization!", __FUNCTION__, field.type->stringName, (int)field.type->enumType);
                                break;
                            }
                        }
                        break;

                    case cJSON_True:
                    case cJSON_False:
                    case cJSON_NULL:
                    case cJSON_Number:
                    case cJSON_Array:
                    case cJSON_Object:
                        {
                            switch (field.type->enumType)
                            {
                            case TypeName::int8_t:
                            case TypeName::int16_t:
                            case TypeName::int32_t:
                            case TypeName::uint8_t:
                            case TypeName::uint16_t:
                            case TypeName::uint32_t:
                            case TypeName::uint:
                            case TypeName::Int:
                            case TypeName::Bool:
                            case TypeName::Float:
                            case TypeName::Double:
                                // case TypeName::eKeys:
                            {
                                size_t smallerSize = sizeof(arrItem->valueint); // #TODO If source is smaller, should the strPointerAddress be offset further?
                                if (smallerSize > field.type->size)
                                {
                                    smallerSize = field.type->size;
                                }

                                if (smallerSize > 0)
                                {
                                    void* fieldAddress = (char*)obj + field.offset;
                                    const void* sourceAddr = (const void*)&arrItem->valueint;
                                    memcpy(fieldAddress, sourceAddr, smallerSize);
                                }
                            }
                            break;

                            default:
                                LOG_ERROR("{0} Unsupported field type {1}({2}) for deserialization!", __FUNCTION__, field.type->stringName, (int)field.type->enumType);
                                break;
                            }
                        }
                        break;

                    default:
                        LOG_ERROR("{0} Unsupported cJSON type {1} for deserialization!", __FUNCTION__, arrItem->type);
                        break;
                    }
                }
            }
        }

    }

    void ConfigHelper::LoadConfigData()
    {
        const char* configFilePath = ConfigsFolderPath(null_config); // #TODO How to handle file names?
        LoadConfigData(configFilePath);
    }

    void ConfigHelper::LoadConfigData(std::string configFilePath)
    {
        // #TODO Dependencies on proper data is fragile. At least have exception handling with more info, or find another way of ensuring proper data
        const cJSON* root = OpencJSONStream(configFilePath.c_str());
        if (root == nullptr)
        {
            LOG_ERROR("Unable to load json for file {0}", configFilePath);
            return;
        }

        // 3 Ways to deserialize
        // Serialization::DeserializeObject<FrameworkData>(GetItemFromRootByKey(root, TYPENAME_TO_STR(FrameworkData)), m_ConfigData.framework);
        // Serialization::DeserializeObject(root->child, Reflection::GetClass<FrameworkData>(), (void*)&m_ConfigData.framework);
        // Serialization::DeserializeObjectByTypeKey<FrameworkData>(root, m_ConfigData.framework);

        const Reflection::ClassInfo* configDataClassInfo = Reflection::GetClass<ConfigData>();
        for (size_t i = 0; i < configDataClassInfo->fields.size(); i++)
        {
            Reflection::Field configDataField = configDataClassInfo->fields.at(i);
            if (!configDataField.type)
                break;

            switch (configDataField.type->enumType)
            {
            case TypeName::FrameworkData:
                Serialization::DeserializeObject<FrameworkData>(GetItemFromRootByKey(root, TYPENAME_TO_STR(FrameworkData)), m_ConfigData.framework);
                // Serialization::DeserializeObjectByTypeKey<FrameworkData>(root, m_ConfigData.framework);
                // Serialization::DeserializeObject(root->child, Reflection::GetClass<FrameworkData>(), (void*)&m_ConfigData.framework);
                break;

            case TypeName::Libraries:
                Serialization::DeserializeObjectByTypeKey<Libraries>(root, m_ConfigData.libraries);
                // Serialization::DeserializeObject(root->child, Reflection::GetClass<Libraries>(), (void*)&m_ConfigData.libraries);
                break;

            case TypeName::ScenesData:
                Serialization::DeserializeObjectByTypeKey<ScenesData>(root, m_ConfigData.scenesData);
                // Serialization::DeserializeObject(root->child, Reflection::GetClass<ScenesData>(), (void*)&m_ConfigData.scenes);
                break;

            case TypeName::SceneSettings:
                Serialization::DeserializeObjectByTypeKey<SceneSettings>(root, m_ConfigData.sceneSettings);
                // Serialization::DeserializeObject(root->child, Reflection::GetClass<SceneSettings>(), (void*)&m_ConfigData.sceneSettings);
                break;

            case TypeName::Systems:
                Serialization::DeserializeObjectByTypeKey<Systems>(root, m_ConfigData.systems);
                // Serialization::DeserializeObject(root->child, Reflection::GetClass<Systems>(), (void*)&m_ConfigData.systems);
                break;

            case TypeName::EngineSettings:
                Serialization::DeserializeObjectByTypeKey<EngineSettings>(root, m_ConfigData.engineSettings);
                // Serialization::DeserializeObject(root->child, Reflection::GetClass<EngineSettings>(), (void*)&m_ConfigData.engineSettings);
                break;

            // case TypeName::Controls: // #TODO Move out of config data, to user data
            //     Serialization::DeserializeObjectByTypeKey<Controls>(root, m_ConfigData.controls);
            //     // Serialization::DeserializeObject(root->child, Reflection::GetClass<Controls>(), (void*)&m_ConfigData.controls);
            //     break;

            default:
                LOG_ERROR("{0} Unhandled field type {1}(2) for serialization!", __FUNCTION__, configDataField.type->stringName, (int)configDataField.type->enumType);
                break;
            }
        }

        ClosecJSONStream(root);
    }

    void ConfigHelper::LoadUserData(std::string preferencesFilePath)
    {
        const cJSON* root = OpencJSONStream(preferencesFilePath.c_str());
        if (root == nullptr)
        {
            LOG_ERROR("Unable to load json for file {0}", preferencesFilePath);
            return;
        }

        const Reflection::ClassInfo* userDataClassInfo = Reflection::GetClass<UserData>();
        for (size_t i = 0; i < userDataClassInfo->fields.size(); i++)
        {
            Reflection::Field userDataField = userDataClassInfo->fields.at(i);
            if (!userDataField.type)
                break;

            switch (userDataField.type->enumType)
            {
            case TypeName::Controls:
                Serialization::DeserializeObjectByTypeKey<Controls>(root, m_UserData.controls);
                break;

            default:
                LOG_ERROR("{0} Unhandled field type {1}(2) for serialization!", __FUNCTION__, userDataField.type->stringName, (int)userDataField.type->enumType);
                break;
            }
        }

        ClosecJSONStream(root);
    }

    void ConfigHelper::SaveConfigData(ConfigData config)
    {
        m_ConfigData = config;
        SaveConfigData();
    }

    void ConfigHelper::SaveConfigData()
    {
        cJSON* root = CreateObject();

        if (cJSON* framework = CreateArray("FrameworkData"))
        {
            AddItemToArray(framework, CreateNumber("QuickLoad", (int)m_ConfigData.framework.QuickLoad));
            AddItemToArray(framework, CreateNumber("MaxConcurrentThreadCount", m_ConfigData.framework.MaxConcurrentThreadCount));
            AddItemToRoot(root, framework);
        }

        if (cJSON* libraries = CreateArray("Libraries"))
        {
            AddItemToArray(libraries, CreateString("Audio", m_ConfigData.libraries.Audio.c_str()));
            AddItemToArray(libraries, CreateString("Networking", m_ConfigData.libraries.Networking.c_str()));
            AddItemToArray(libraries, CreateString("Physics", m_ConfigData.libraries.Physics.c_str()));
            AddItemToArray(libraries, CreateString("Rendering", m_ConfigData.libraries.Rendering.c_str()));
            AddItemToArray(libraries, CreateString("Window", m_ConfigData.libraries.Window.c_str()));
            AddItemToRoot(root, libraries);
        }

        if (cJSON* scenes = CreateArray("SceneData"))
        {
            for (unsigned int i = 0; i < m_ConfigData.scenesData.fileNames.size(); i++)
            {
                AddItemToArray(scenes, CreateString(std::to_string(i).c_str(), m_ConfigData.scenesData.fileNames[i].c_str()));
            }
            AddItemToRoot(root, scenes);
        }

        if (cJSON* sceneSettings = CreateArray("SceneSettings"))
        {
            AddItemToArray(sceneSettings, CreateNumber("MaxEnabledScenes", m_ConfigData.sceneSettings.MaxEnabledScenes));
            AddItemToRoot(root, sceneSettings);
        }

        if (cJSON* systems = CreateArray("Systems"))
        {
            AddItemToArray(systems, CreateBool("AudioEnabled", m_ConfigData.systems.AudioEnabled));
            AddItemToArray(systems, CreateBool("NetworkingEnabled", m_ConfigData.systems.NetworkingEnabled));
            AddItemToArray(systems, CreateBool("PhysicsEnabled", m_ConfigData.systems.PhysicsEnabled));
            AddItemToArray(systems, CreateBool("ConsoleOutputWindowEnabled", m_ConfigData.systems.ConsoleOutputWindowEnabled));
            AddItemToRoot(root, systems);
        }

        if (cJSON* engineSettings = CreateArray("EngineSettings"))
        {
            AddItemToArray(engineSettings, CreateBool("LimitFramerate", m_ConfigData.engineSettings.LimitFramerate));
            AddItemToArray(engineSettings, CreateNumber("MaxFramesPerSecond", (int)m_ConfigData.engineSettings.MaxFramesPerSecond));
            AddItemToRoot(root, engineSettings);
        }

        const char* configFilePath = ConfigsFolderPath(null_config);
        PrintRootObjectToFile(configFilePath, root); // #TODO Take in config file path as arg
    }

    void ConfigHelper::SaveUserData()
    {
        cJSON* root = CreateObject();

        // #TODO Controls data format changed and the below code will throw an exception until updated
        return;

        if (cJSON* controls = CreateArray("Controls"))
        {
            AddItemToArray(controls, CreateString(FULL_VARIABLE_TO_STRING(m_UserData.controls.Camera_MoveForward), (char*)&m_UserData.controls.Camera_MoveForward));
            AddItemToArray(controls, CreateString(FULL_VARIABLE_TO_STRING(m_UserData.controls.Camera_MoveBackward), (char*)&m_UserData.controls.Camera_MoveBackward));
            AddItemToArray(controls, CreateString(FULL_VARIABLE_TO_STRING(m_UserData.controls.Camera_MoveLeft), (char*)&m_UserData.controls.Camera_MoveLeft));
            AddItemToArray(controls, CreateString(FULL_VARIABLE_TO_STRING(m_UserData.controls.Camera_MoveRight), (char*)&m_UserData.controls.Camera_MoveRight));
            AddItemToArray(controls, CreateString(FULL_VARIABLE_TO_STRING(m_UserData.controls.Camera_MoveUp), (char*)&m_UserData.controls.Camera_MoveUp));
            AddItemToArray(controls, CreateString(FULL_VARIABLE_TO_STRING(m_UserData.controls.Camera_MoveDown), (char*)&m_UserData.controls.Camera_MoveDown));
            AddItemToArray(controls, CreateString(FULL_VARIABLE_TO_STRING(m_UserData.controls.Camera_RotateLeft), (char*)&m_UserData.controls.Camera_RotateLeft));
            AddItemToArray(controls, CreateString(FULL_VARIABLE_TO_STRING(m_UserData.controls.Camera_RotateRight), (char*)&m_UserData.controls.Camera_RotateRight));
            AddItemToRoot(root, controls);
        }

        const char* preferencesFilePath = PreferencesFolderPath(null_preferences);
        PrintRootObjectToFile(preferencesFilePath, root); // #TODO Take in config file path as arg
    }

}
