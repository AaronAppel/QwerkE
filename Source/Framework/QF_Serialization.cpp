#include "QF_Serialization.h"

#include "Libraries/cJSON/QC_cJSON.h"

#include "QF_eKeys.h"
#include "QF_Log.h"

// #TODO Look at removing from QwerkE namespace, or have a #using QwerkE just in this .cpp.
// Helps with porting to a stand alone library, and encourages type indifference/independence
namespace QwerkE {

    namespace Serialization
    {

        void DeserializeJsonString(const cJSON* jsonObj, const Mirror::Field& field, void* obj);

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
                            case MirrorTypes::EngineSettings: // DESERIALIZE_CASE_FOR_CLASS(EngineSettings)
                                DeserializeJsonArrayToObject(jsonStructArr[j], Mirror::InfoForClass<EngineSettings>(), ((char*)obj + field.offset));
                                break;

                                DESERIALIZE_CASE_FOR_CLASS(ProjectSettings)
                                DESERIALIZE_CASE_FOR_CLASS(SceneViewerData)
                                DESERIALIZE_CASE_FOR_CLASS(UserSettings)

                            case MirrorTypes::m_vector_string:
                                {
                                    std::vector<std::string>* strings = (std::vector<std::string>*)((char*)obj + field.offset);
                                    const std::vector<cJSON*> jsonStrings = GetAllItemsFromArray(jsonStructArr[j]);
                                    strings->reserve(jsonStrings.size());

                                    if (strings)
                                    {
                                        for (size_t i = 0; i < jsonStrings.size(); i++)
                                        {
                                            std::string str = jsonStrings[i]->valuestring;
                                            strings->push_back(str);
                                        }
                                    }
                                    else
                                    {
                                        LOG_WARN("{0} Unable to deserialize type of {1}({2})", __FUNCTION__, field.type->stringName, (int)field.type->enumType);
                                    }
                                }
                                break;

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

        void DeserializeJsonString(const cJSON* jsonObj, const Mirror::Field& field, void* obj)
        {
            if (!jsonObj || !jsonObj->valuestring || (jsonObj->type != cJSON_String) || !obj)
            {
                LOG_ERROR("{0} Null/invalid argument passed!", __FUNCTION__);
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

        void SerializeField(cJSON* objJson, const Mirror::Field& field, const void* obj);

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
                    case MirrorTypes::EngineSettings: // SERIALIZE_CASE_FOR_CLASS(EngineSettings)
                        SerializeJsonObject(arr, Mirror::InfoForClass<EngineSettings>(), (char*)obj + field.offset);
                        break;

                        SERIALIZE_CASE_FOR_CLASS(ProjectSettings)
                        SERIALIZE_CASE_FOR_CLASS(SceneViewerData)
                        SERIALIZE_CASE_FOR_CLASS(UserSettings)

                    case MirrorTypes::m_vector_string:
                        {
                            const ProjectSettings* projectSettings = (ProjectSettings*)obj;

                            std::vector<std::string>* strings = (std::vector<std::string>*)((char*)obj + field.offset);

                            if (strings)
                            {
                                for (size_t i = 0; i < strings->size(); i++)
                                {
                                    std::string str = strings->at(i);
                                    AddItemToArray(arr, CreateString(std::to_string(i).c_str(), str.c_str()));
                                }
                            }
                            else
                            {
                                LOG_WARN("{0} Unable to deserialize type of {1}({2})", __FUNCTION__, field.type->stringName, (int)field.type->enumType);
                            }
                        }
                        break;

                    default:
                        LOG_WARN("{0} Unsupported user defined field type {1}({2}) for serialization!", __FUNCTION__, field.type->stringName, (int)field.type->enumType);
                        break;
                    }
                }
                else
                {
                    SerializeField(objJson, field, obj);
                }
            }
        }

        void SerializeField(cJSON* objJson, const Mirror::Field& field, const void* obj)
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

            switch (field.type->enumType)
            {
            case MirrorTypes::m_string:
                {
                    const std::string* fieldAddress = (const std::string*)((char*)obj + field.offset);
                    AddItemToArray(objJson, CreateString(field.name.c_str(), fieldAddress->data()));
                }
                break;

            case MirrorTypes::m_charPtr:
            case MirrorTypes::m_constCharPtr:
                {
                    const char* fieldAddress = *(const char**)((char*)obj + field.offset);
                    AddItemToArray(objJson, CreateString(field.name.c_str(), fieldAddress));
                }
                break;

            case MirrorTypes::m_char:
            case MirrorTypes::eKeys:
                {
                    char* stringAddress = (char*)obj + field.offset;
                    AddItemToArray(objJson, CreateString(field.name.c_str(), stringAddress));
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