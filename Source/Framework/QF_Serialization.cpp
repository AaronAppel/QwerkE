
#include "Libraries/cJSON/QC_cJSON.h"

#include "QC_Reflection.h"

#include "QF_Defines.h"
#include "QF_Input.h"
#include "QF_Log.h"

#include "QF_ConfigHelper.h" // #TODO Remove

namespace QwerkE {

    namespace Serialization
    {

#if _WIN32
        typedef INT32 PlatformPointer; // #TODO Handle pointer size on different systems better
#elif _WIN64
        typedef INT64 PlatformPointer;
#pragma warning "Implement 64 bit support!"
#endif
        void DeserializeJsonString(const cJSON* jsonObj, const Reflection::Field& field, void* obj)
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
            case ReflectionType::r_string:
            {
                size_t smallerSize = strlen(jsonObj->valuestring);

                std::string* fieldAddress = reinterpret_cast<std::string*>((char*)obj + field.offset);
                if (smallerSize > fieldAddress->size())
                {
                    smallerSize = field.type->size;
                }

                if (smallerSize > 0)
                {
                    const void* sourceAddr = (const void*)jsonObj->valuestring;
                    memcpy((void*)fieldAddress->data(), sourceAddr, smallerSize);
                }
            }
            break;

            case ReflectionType::r_char:
            {
                jsonObj->valuestring;
                void* fieldAddress = (char*)obj + field.offset;
                memcpy(fieldAddress, jsonObj->valuestring, sizeof(void*));
            }
            break;

            case ReflectionType::r_charPtr:
            case ReflectionType::r_constCharPtr:
            {
                // #TODO Validate field.type->size
                PlatformPointer* writeAddress = (PlatformPointer*)((char*)obj + field.offset);

                // if (writeAddress) // #TODO How to know if casted memory address is valid?
                {
                    // #TODO How to know allocation function new vs new [] vs malloc?
                    // delete[] writeAddress;
                }

                *writeAddress = (PlatformPointer)DeepCopyString(jsonObj->valuestring);
            }
            break;

            case ReflectionType::eKeys: // #TODO Consider using a number value instead of a string
            {
                const char result = jsonObj->valuestring[0];
                if (field.type->size >= sizeof(const char))
                {
                    void* fieldAddress = (char*)obj + field.offset;
                    memcpy(fieldAddress, &result, sizeof(const char));
                }
                else
                {
                    LOG_WARN("{0} Unable to deserialize value for cJSON value {1} and field type {2}", __FUNCTION__, jsonObj->string, field.type->stringName);
                }
            }
            break;

            default:
                LOG_ERROR("{0} Unsupported field type {1}({2}) for cJSON deserialization!", __FUNCTION__, field.type->stringName, (int)field.type->enumType);
                break;
            }
        }

        void DeserializeJsonNumber(const cJSON* jsonObj, const Reflection::Field& field, void* obj)
        {
            if (!jsonObj || (jsonObj->type == cJSON_String) || (jsonObj->type == cJSON_NULL) || !obj)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            switch (field.type->enumType)
            {
            case ReflectionType::r_int8_t:
            case ReflectionType::r_int16_t:
            case ReflectionType::r_int32_t:
            case ReflectionType::r_int64_t:
            case ReflectionType::r_uint8_t:
            case ReflectionType::r_uint16_t:
            case ReflectionType::r_uint32_t:
            case ReflectionType::r_uint64_t:
            case ReflectionType::r_int:
            case ReflectionType::r_bool:
            case ReflectionType::r_float:
            case ReflectionType::r_double:
            // case ReflectionType::eKeys: // #TODO Transition to use a number instead of a string
                {
                    // #TODO Review writing too much memory, and alignment (right vs left, big vs little endian)
                    size_t smallerSize = sizeof(jsonObj->valueint); // #TODO If source is smaller, should the strPointerAddress be offset further?
                    if (smallerSize > field.type->size)
                    {
                        smallerSize = field.type->size;
                    }

                    if (smallerSize > 0)
                    {
                        void* fieldAddress = (char*)obj + field.offset;
                        const void* sourceAddr = (const void*)&jsonObj->valueint;
                        memcpy(fieldAddress, sourceAddr, smallerSize);
                    }
                }
                break;

            default:
                LOG_ERROR("{0} Unsupported field type {1}({2}) for deserialization!", __FUNCTION__, field.type->stringName, (int)field.type->enumType);
                break;
            }
        }

        void DeserializeJsonObject(const cJSON* objJson, const Reflection::ClassInfo* objClassInfo, void* obj)
        {
            if (!objJson)
            {
                LOG_ERROR("{0} {1} is null!", __FUNCTION__, VARNAME_TO_STR(objJson));
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
            const std::vector<cJSON*> arr = GetAllItemsFromArray(objJson);
            for (size_t i = 0; i < arr.size(); i++)
            {
                const cJSON* arrItem = arr[i];

                for (size_t i = 0; i < objClassInfo->fields.size(); i++)
                {
                    const Reflection::Field& field = objClassInfo->fields[i];

                    if (!field.type)
                        break; // #TODO Review #NOTE Break early if fields become null (and future values are null) to save iterations

                    if (strcmp(field.name.c_str(), arrItem->string) != 0) // #TODO Type name only handles 1 instance, and isn't dynamic enough
                        continue;

                    switch (arrItem->type)
                    {
                    case cJSON_String:
                        DeserializeJsonString(arrItem, field, obj);
                        break;

                    case cJSON_True:
                    case cJSON_False:
                    case cJSON_NULL:
                    case cJSON_Number:
                    case cJSON_Array:
                    case cJSON_Object:
                        DeserializeJsonNumber(arrItem, field, obj);
                        break;

                    default:
                        LOG_ERROR("{0} Unsupported cJSON type {1} for deserialization!", __FUNCTION__, arrItem->type);
                        break;
                    }
                }
            }
        }

        void SerializeJsonObject(cJSON* objJson, const Reflection::ClassInfo* objClassInfo, void* obj)
        {
            if (!objJson || !objClassInfo || !obj)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            for (size_t i = 0; i < objClassInfo->fields.size(); i++)
            {
                const Reflection::Field& field = objClassInfo->fields[i];

                if (!field.type)
                    break; // #TODO Review #NOTE Break early if fields become null (and future values are null) to save iterations

                switch (field.type->enumType)
                {
                case ReflectionType::r_string:
                    {
                        const std::string* fieldAddress = (const std::string*)((char*)obj + field.offset);
                        AddItemToArray(objJson, CreateString(field.name.c_str(), fieldAddress->c_str())); // #TODO cJSON_AddItemToArray
                    }
                    break;

                case ReflectionType::r_charPtr:
                case ReflectionType::r_constCharPtr:
                    {
                        const char* fieldAddress = *(const char**)((char*)obj + field.offset);
                        AddItemToArray(objJson, CreateString(field.name.c_str(), fieldAddress)); // #TODO cJSON_AddItemToArray
                    }
                    break;

                case ReflectionType::r_char:
                case ReflectionType::eKeys: // #TODO Consider using a number value instead of a string
                    // SerializeToJsonString(objJson, field, obj);
                    {
                        char* stringAddress = (char*)obj + field.offset;
                        AddItemToArray(objJson, CreateString(field.name.c_str(), stringAddress)); // #TODO cJSON_AddItemToArray
                    }
                    break;

                case ReflectionType::r_bool:
                    {
                        int* boolAddress = (int*)((char*)obj + field.offset);
                        AddItemToArray(objJson, CreateBool(field.name.c_str(), *boolAddress));
                    }
                    break;

                case ReflectionType::r_int8_t:
                case ReflectionType::r_int16_t:
                case ReflectionType::r_int32_t:
                case ReflectionType::r_int64_t:
                case ReflectionType::r_uint8_t:
                case ReflectionType::r_uint16_t:
                case ReflectionType::r_uint32_t:
                case ReflectionType::r_uint64_t:
                case ReflectionType::r_int:
                case ReflectionType::r_float:
                case ReflectionType::r_double:
                    // case ReflectionType::eKeys: // #TODO Transition to use a number instead of a string
                    // SerializeToJsonNumber(objJson, field, obj);
                    {
                        int* numberAddress = (int*)((char*)obj + field.offset); // #TODO Support more type sizes
                        AddItemToArray(objJson, CreateNumber(field.name.c_str(), *numberAddress));
                    }
                    break;

                default:
                    LOG_ERROR("{0} Unsupported field type {1}({2}) for deserialization!", __FUNCTION__, field.type->stringName, (int)field.type->enumType);
                    break;
                }
            }
        }

    }

}