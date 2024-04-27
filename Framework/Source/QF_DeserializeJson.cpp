#include "QF_Serialization.h"

#include "QF_Buffer.h"

namespace QwerkE {

    namespace Serialization {

#define DEBUG_LEVEL 1

        void local_DeserializePrimitive(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, bool useNameString = false);
        void local_DeserializeClass(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj);
        void local_DeserializeCollection(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, const std::string& name);

        void DeserializeFromJson(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            switch (objTypeInfo->category)
            {
            case Mirror::TypeInfoCategories::TypeInfoCategory_Primitive:
                local_DeserializePrimitive(objJson, objTypeInfo, obj); break;
            case Mirror::TypeInfoCategories::TypeInfoCategory_Class:
                local_DeserializeClass(objJson, objTypeInfo, obj); break;
            case Mirror::TypeInfoCategories::TypeInfoCategory_Collection:
                local_DeserializeCollection(objJson, objTypeInfo, obj, objTypeInfo->stringName); break;
            case Mirror::TypeInfoCategories::TypeInfoCategory_Pointer: // #TODO Look to remove
                break;

            case Mirror::TypeInfoCategories::TypeInfoCategory_Invalid:
            default:
                ASSERT(false, "Unsupported category!");
                break;
            }
        }

        template <typename T, typename U>
        void local_Write(void* destination, const void* source) // #TODO Deprecate
        {
            // #NOTE Value of this function is in type conversion, rather than writing bytes of data that isn't in required format
            T* objAddress = (T*)destination;
            *objAddress = *(U*)source;
        }

        void local_DeserializePrimitive(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, bool isPairFirst)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            switch (objTypeInfo->enumType)
            {
            case MirrorTypes::m_string:
                {
                    std::string* str = (std::string*)obj;
                    const char* result = isPairFirst ? objJson->string : objJson->valuestring;
                    if (!result) // #TODO Take out safety check with a better solution
                    {
                        result = isPairFirst ? objJson->valuestring : objJson->string;
                    }
                    *str = result;
                }
                break;
            case MirrorTypes::m_charPtr:
            case MirrorTypes::m_constCharPtr:
                {
                    const char** str = (const char**)obj;
                    *str = _strdup(objJson->valuestring);
                }
                break;

            case MirrorTypes::m_uint64_t: // #NOTE Storing uint64 as string
                {
                    u64* valuePtr = (u64*)obj;
                    const u64 temp = std::stoull(objJson->valuestring);
                    *valuePtr = temp;
                    // #TODO Try using objJson->valuedouble and a memcpy to see if that works
                }
                break;
            case MirrorTypes::m_int64_t: // #NOTE Storing int64 as string
                {
                    s64* valuePtr = (s64*)obj;
                    const s64 temp = std::stoll(objJson->valuestring);
                    *valuePtr = temp;
                    // #TODO Try using objJson->valuedouble and a memcpy to see if that works
                }
                break;

            case MirrorTypes::m_float:
                local_Write<float, double>(obj, &objJson->valuedouble); break;

            default:
                // if (cJSON_Number == objJson->type || cJSON_True == objJson->type || cJSON_False == objJson->type)
                if (objJson->type | cJSON_Number | cJSON_True | cJSON_False)
                {
                    // #TODO Potential bug writing to fieldAddress with size less than objJson->valueint
                    // #TODO Try to use memcpy for bool, float, double, and char assignments/cases
                    void* fieldAddress = ((char*)obj);
                    void* sourceAddress = (void*)&objJson->valueint;

                    if (objTypeInfo->size > 4)
                    {
                        sourceAddress = (void*)&objJson->valuedouble;
                    }
                    if (MirrorTypes::m_char == objTypeInfo->enumType ||
                        MirrorTypes::eKeys == objTypeInfo->enumType)
                    {
                        sourceAddress = (void*)&objJson->valuestring[0];
                    }
                    memcpy(fieldAddress, sourceAddress, objTypeInfo->size);
                }
                else
                {
                    // #TODO LOG_ERROR
                }
                break;
            }
        }

        void local_DeserializeClass(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj)
        {
            const cJSON* iterator = objJson->child;
            u8 index = 0;

            while (iterator)
            {
                const Mirror::Field& field = objTypeInfo->fields[index];

#if DEBUG_LEVEL 1
                if (strcmp(field.name.c_str(), iterator->string) != 0)
                    LOG_WARN("{0} Mismatched names!", __FUNCTION__);
#endif

                if (field.serializationFlags & Mirror::FieldSerializationFlags::_InspectorOnly)
                    continue;

                if (field.typeInfo->isCollection())
                {
                    // #NOTE Needed to pass name
                    local_DeserializeCollection(iterator, field.typeInfo, (char*)obj + field.offset, field.name);
                }
                else
                {
                    DeserializeFromJson(iterator, field.typeInfo, (char*)obj + field.offset);
                }

                iterator = iterator->next;
                ++index;
            }
        }

        void local_DeserializeCollection(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            const Mirror::TypeInfo* const elementFirstTypeInfo = objTypeInfo->CollectionTypeInfoFirst();
            Buffer elementFirstBuffer(elementFirstTypeInfo->size);

            const Mirror::TypeInfo* const elementSecondInfo = objTypeInfo->CollectionTypeInfoSecond();
            Buffer elementSecondBuffer(elementSecondInfo ? elementSecondInfo->size : 0);

            const cJSON* it = objJson->child;
            size_t index = 0;

            while (it)
            {
                const bool hasConstructionDependency = false;
                // #TODO Handle case where string needs construction, but not basic/primitive types
                if (!hasConstructionDependency &&
                    (!elementFirstTypeInfo->isPrimitive() || MirrorTypes::m_string == elementFirstTypeInfo->enumType))
                {
                    ASSERT(elementFirstTypeInfo->typeConstructorFunc, "Construct function is null!");
                    elementFirstTypeInfo->typeConstructorFunc(elementFirstBuffer.As<void>());
                }

                DeserializeFromJson(it, elementFirstTypeInfo, elementFirstBuffer.As<void>());

                if (objTypeInfo->isPair())
                {
                    DeserializeFromJson(it, elementSecondInfo, elementSecondBuffer.As<void>());
                }

                objTypeInfo->CollectionAppend(obj, index, elementFirstBuffer.As<void>(), elementSecondBuffer.As<void>());

                if (hasConstructionDependency &&
                    (!elementFirstTypeInfo->isPrimitive() || MirrorTypes::m_string == elementFirstTypeInfo->enumType))
                {
                    ASSERT(elementFirstTypeInfo->typeConstructorFunc, "Construct function is null!");
                    elementFirstTypeInfo->typeConstructorFunc(elementFirstBuffer.As<void>()); // #TODO Add/supply constructor argument data
                }

                it = it->next;
                ++index;
            }
        }

    }

}
