#include "QF_ConfigHelper.h"

#include "Libraries/cJSON/QC_cJSON.h"

#include "QC_Reflection.h"

#include "QF_Defines.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Serialization.h"

namespace QwerkE {

    ConfigData ConfigHelper::m_ConfigData = ConfigData();
    UserData ConfigHelper::m_UserData = UserData();

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
                break; // #NOTE Future fields will also have null types

            switch (configDataField.type->enumType)
            {
            case ReflectionType::FrameworkData:
                Serialization::DeserializeObject<FrameworkData>(GetItemFromRootByKey(root, TYPENAME_TO_STR(FrameworkData)), m_ConfigData.framework);
                // Serialization::DeserializeObjectByTypeKey<FrameworkData>(root, m_ConfigData.framework);
                // Serialization::DeserializeObject(root->child, Reflection::GetClass<FrameworkData>(), (void*)&m_ConfigData.framework);
                break;

            case ReflectionType::Libraries:
                Serialization::DeserializeObjectByTypeKey<Libraries>(root, m_ConfigData.libraries);
                // Serialization::DeserializeObject(root->child, Reflection::GetClass<Libraries>(), (void*)&m_ConfigData.libraries);
                break;

            case ReflectionType::ScenesData:
                Serialization::DeserializeObjectByTypeKey<ScenesData>(root, m_ConfigData.scenesData);
                // Serialization::DeserializeObject(root->child, Reflection::GetClass<ScenesData>(), (void*)&m_ConfigData.scenes);
                break;

            case ReflectionType::SceneSettings:
                Serialization::DeserializeObjectByTypeKey<SceneSettings>(root, m_ConfigData.sceneSettings);
                // Serialization::DeserializeObject(root->child, Reflection::GetClass<SceneSettings>(), (void*)&m_ConfigData.sceneSettings);
                break;

            case ReflectionType::Systems:
                Serialization::DeserializeObjectByTypeKey<Systems>(root, m_ConfigData.systems);
                // Serialization::DeserializeObject(root->child, Reflection::GetClass<Systems>(), (void*)&m_ConfigData.systems);
                break;

            case ReflectionType::EngineSettings:
                Serialization::DeserializeObjectByTypeKey<EngineSettings>(root, m_ConfigData.engineSettings);
                // Serialization::DeserializeObject(root->child, Reflection::GetClass<EngineSettings>(), (void*)&m_ConfigData.engineSettings);
                break;

            // case ReflectionType::r_Controls: // #TODO Move out of config data, to user data
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
            case ReflectionType::Controls:
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
