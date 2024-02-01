#include "QF_Serialization.h"

#include "Libraries/cJSON/QC_cJSON.h"

#include "QC_Reflection.h"

#include "QF_Debug.h"
#include "QF_Defines.h"
#include "QF_Input.h"
#include "QF_Log.h"

// #TODO Look at removing from QwerkE namespace, or have a #using QwerkE just in this .cpp.
// Helps with porting to a stand alone library, and encourages type indifference/independence
namespace QwerkE {

    namespace Serialization
    {

        void DeserializeJsonString(const cJSON* jsonObj, const Mirror::Field& field, void* obj)
        {
            // #TODO Worry about the null terminating character '\0'
            // #TODO Handle case where target/strPointerAddress is null or empty

            if (!jsonObj || !jsonObj->valuestring || (jsonObj->type != cJSON_String) || !obj)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            switch (field.type->enumType)
            {
            case MirrorTypes::m_string:
                {
                    std::string* fieldAddress = (std::string*)((char*)obj + field.offset);
                    *fieldAddress = jsonObj->valuestring;
                }
                break;

            case MirrorTypes::eKeys:
            case MirrorTypes::m_char:
                {
                    char* fieldAddress = (char*)obj + field.offset;
                    *fieldAddress = jsonObj->valuestring[0];
                }
                break;

            case MirrorTypes::m_charPtr:
            case MirrorTypes::m_constCharPtr:
                {
                    char** fieldAddress = (char**)((char*)obj + field.offset);
                    *fieldAddress = (char*)_strdup(jsonObj->valuestring); // #TODO Handle malloc with matching free
                }
                break;

            default:
                LOG_ERROR("{0} Unsupported field type {1}({2}) for cJSON deserialization!", __FUNCTION__, field.type->stringName, (int)field.type->enumType);
                break;
            }
        }

#define DESERIALIZE_CASE_FOR_CLASS(ClassType) \
case MirrorTypes::ClassType: \
    DeserializeJsonArrayToObject(jsonStructArr[j], Mirror::InfoForClass<ClassType>(), ((char*)obj + field.offset)); \
    break; \

        void DeserializeJsonArrayToObject(const cJSON* objJson, const Mirror::ClassInfo* objClassInfo, void* obj)
        {
            if (!objJson || !objClassInfo || !obj)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (objJson->type != cJSON_Array)
            {
                LOG_ERROR("{0} Only takes JSON objects! Type was {1}", __FUNCTION__, (int)objJson->type);
                return;
            }

            const std::vector<cJSON*>& jsonStructArr = GetAllItemsFromArray(objJson);

            for (size_t i = 0; i < objClassInfo->fields.size(); i++)
            {
                const Mirror::Field& field = objClassInfo->fields[i];

                for (size_t j = 0; j < jsonStructArr.size(); j++)
                {
                    if (jsonStructArr[j]->type == cJSON_Array)
                    {
                        if (strcmp(field.type->stringName.c_str(), jsonStructArr[j]->string) == 0)
                        {
                            if (field.type->enumType > MirrorTypes::m_PRIMITIVE_TYPES)
                            {
                                LOG_ERROR("{0} Primitive type!", __FUNCTION__);
                                continue;
                            }

                            switch (field.type->enumType)
                            {
                            case MirrorTypes::ConfigData: // DESERIALIZE_CASE_FOR_CLASS(ConfigData)
                                DeserializeJsonArrayToObject(jsonStructArr[j], Mirror::InfoForClass<ConfigData>(), ((char*)obj + field.offset));
                                break;

                                DESERIALIZE_CASE_FOR_CLASS(ConfiguredGameKeys)
                                DESERIALIZE_CASE_FOR_CLASS(Controls)
                                DESERIALIZE_CASE_FOR_CLASS(EngineSettings)
                                DESERIALIZE_CASE_FOR_CLASS(FrameworkData)
                                DESERIALIZE_CASE_FOR_CLASS(Libraries)
                                DESERIALIZE_CASE_FOR_CLASS(MenuBarData)
                                DESERIALIZE_CASE_FOR_CLASS(RepeatedStruct)
                                DESERIALIZE_CASE_FOR_CLASS(ScenesList)
                                DESERIALIZE_CASE_FOR_CLASS(SceneSettings)
                                DESERIALIZE_CASE_FOR_CLASS(SceneViewerData)
                                DESERIALIZE_CASE_FOR_CLASS(Systems)
                                DESERIALIZE_CASE_FOR_CLASS(UiData)
                                DESERIALIZE_CASE_FOR_CLASS(UserData)

                            default:
                                LOG_WARN("{0} User defined type {1}({2}) not supported here!", __FUNCTION__, field.type->stringName, (int)field.type->enumType);
                                break;
                            }
                        }
                    }
                    else // Lowest level (primitives)
                    {
                        if (strcmp(field.name.c_str(), jsonStructArr[j]->string) == 0)
                        {
                            if (field.type->enumType < MirrorTypes::m_PRIMITIVE_TYPES)
                            {
                                LOG_ERROR("{0} Not a primitive type!", __FUNCTION__);
                                continue;
                            }

                            ProjectData* projectData = (ProjectData*)obj;
                            FrameworkData* frameworkData = (FrameworkData*)obj;
                            Libraries* libraries = (Libraries*)obj;

                            switch (jsonStructArr[j]->type)
                            {
                            case cJSON_String:
                                DeserializeJsonString(jsonStructArr[j], field, obj);
                                break;

                            case cJSON_True:
                            case cJSON_False:
                                {
                                    bool* fieldAddress = ((bool*)obj + field.offset);
                                    *fieldAddress = jsonStructArr[j]->type == cJSON_True;
                                }
                                break;

                            case cJSON_Number:
                                {
                                    size_t bytesToWrite = sizeof(jsonStructArr[j]->valueint);
                                    if (field.type->size < bytesToWrite)
                                    {
                                        bytesToWrite = field.type->size;
                                    }

                                    void* fieldAddress = ((char*)obj + field.offset);
                                    memcpy(fieldAddress, &jsonStructArr[j]->valueint, bytesToWrite);
                                }
                                break;

                            case cJSON_Array:
                            case cJSON_Object:
                            case cJSON_NULL:
                            default:
                                LOG_WARN("{0} JSON type {1} not supported here!", __FUNCTION__, (int)jsonStructArr[j]->type);
                                break;
                            }
                        }
                    }
                }
            }
        }

        void SerializeJsonField(cJSON* objJson, const Mirror::Field& field, const void* obj);

#define SERIALIZE_CASE_FOR_CLASS(ClassType) \
case MirrorTypes::ClassType: \
    SerializeJsonObject(arr, Mirror::InfoForClass<ClassType>(), (char*)obj + field.offset); \
    break; \

        void SerializeJsonObject(cJSON* objJson, const Mirror::ClassInfo* objClassInfo, const void* obj)
        {
            if (!objJson || !objClassInfo || !obj)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            for (size_t i = 0; i < objClassInfo->fields.size(); i++)
            {
                const Mirror::Field& field = objClassInfo->fields[i];

                if (!field.type)
                    break;

                if (field.type->enumType < MirrorTypes::m_PRIMITIVE_TYPES)
                {
                    cJSON* arr = CreateArray(field.type->stringName.c_str());
                    cJSON_AddItemToArray(objJson->child, arr);

                    switch (field.type->enumType)
                    {
                    case MirrorTypes::ConfigData: // SERIALIZE_CASE_FOR_CLASS(ConfigData)
                        SerializeJsonObject(arr, Mirror::InfoForClass<ConfigData>(), (char*)obj + field.offset);
                        break;

                        SERIALIZE_CASE_FOR_CLASS(ConfiguredGameKeys)
                        SERIALIZE_CASE_FOR_CLASS(Controls)
                        SERIALIZE_CASE_FOR_CLASS(EngineSettings)
                        SERIALIZE_CASE_FOR_CLASS(FrameworkData)
                        SERIALIZE_CASE_FOR_CLASS(Libraries)
                        SERIALIZE_CASE_FOR_CLASS(MenuBarData)
                        SERIALIZE_CASE_FOR_CLASS(ProjectData)
                        SERIALIZE_CASE_FOR_CLASS(RepeatedStruct)
                        SERIALIZE_CASE_FOR_CLASS(SceneSettings)
                        SERIALIZE_CASE_FOR_CLASS(ScenesList)
                        SERIALIZE_CASE_FOR_CLASS(SceneViewerData)
                        SERIALIZE_CASE_FOR_CLASS(Systems)
                        SERIALIZE_CASE_FOR_CLASS(UiData)
                        SERIALIZE_CASE_FOR_CLASS(UserData)

                    default:
                        LOG_WARN("{0} Unsupported user defined field type {1}({2}) for serialization!", __FUNCTION__, field.type->stringName, (int)field.type->enumType);
                        break;
                    }
                }
                else
                {
                    Libraries* libraries = (Libraries*)obj;
                    SerializeJsonField(objJson, field, obj);
                }
            }
        }

        void SerializeJsonField(cJSON* objJson, const Mirror::Field& field, const void* obj)
        {
            if (!objJson || !obj)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (!field.type || field.type->enumType < MirrorTypes::m_PRIMITIVE_TYPES)
            {
                LOG_ERROR("{0} Invalid field passed!", __FUNCTION__);
                return;
            }

            // #TODO Use field.type->bytesToWrite to verify and write correct amount of data
            switch (field.type->enumType)
            {
            case MirrorTypes::m_string:
                {
                    Libraries* libraries = (Libraries*)obj;
                    const std::string* fieldAddress = (const std::string*)((char*)obj + field.offset);
                    AddItemToArray(objJson, CreateString(field.name.c_str(), fieldAddress->data())); // #TODO cJSON_AddItemToArray
                }
                break;

            case MirrorTypes::m_charPtr:
            case MirrorTypes::m_constCharPtr:
                {
                    const char* fieldAddress = *(const char**)((char*)obj + field.offset);
                    AddItemToArray(objJson, CreateString(field.name.c_str(), fieldAddress)); // #TODO cJSON_AddItemToArray
                }
                break;

            case MirrorTypes::m_char:
            case MirrorTypes::eKeys:
                {
                    char* stringAddress = (char*)obj + field.offset;
                    AddItemToArray(objJson, CreateString(field.name.c_str(), stringAddress)); // #TODO cJSON_AddItemToArray
                }
                break;

            case MirrorTypes::m_bool:
                {
                    bool* boolAddress = (bool*)((char*)obj + field.offset);
                    AddItemToArray(objJson, CreateBool(field.name.c_str(), *boolAddress));
                }
                break;

            case MirrorTypes::m_int8_t:
            case MirrorTypes::m_int16_t:
            case MirrorTypes::m_int32_t:
            case MirrorTypes::m_int64_t:
            case MirrorTypes::m_uint8_t:
            case MirrorTypes::m_uint16_t:
            case MirrorTypes::m_uint32_t:
            case MirrorTypes::m_uint64_t:
            case MirrorTypes::m_int:
            case MirrorTypes::m_float:
            case MirrorTypes::m_double:
                {
                    int* numberAddress = (int*)((char*)obj + field.offset);
                    AddItemToArray(objJson, CreateNumber(field.name.c_str(), *numberAddress));
                }
                break;

            default:
                LOG_ERROR("{0} Unsupported field type {1}({2}) for serialization!", __FUNCTION__, field.type->stringName, (int)field.type->enumType);
                break;
            }
        }

    }

}