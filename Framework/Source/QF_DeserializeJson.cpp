#include "QF_Serialization.h"

#include "QF_ScriptHelpers.h"

#include "QF_ComponentHelpers.h"

// Editor types
#include "../Editor/Source/QE_EditorWindowHelpers.h"

namespace QwerkE {

    namespace Serialization {

        void local_DeserializePrimitive(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj, bool useNameString = false);
        void local_DeserializeClass(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj);
        void local_DeserializeCollection(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj, const std::string& name);

        void DeserializeFromJson(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            // #TODO Add safety checks

            if (objTypeInfo->isPrimitive())
            {
                // if (objTypeInfo->isPointer) // #TODO Move outside
                {
                    // local_DeserializePrimitive(objJson, objTypeInfo, *(void**)obj);
                }
                // else
                {
                    local_DeserializePrimitive(objJson, objTypeInfo, obj);
                }
            }
            else if (objTypeInfo->isClass)
            {
                local_DeserializeClass(objJson, objTypeInfo, obj);
            }
            else if (objTypeInfo->isCollection())
            {
                local_DeserializeCollection(objJson, objTypeInfo, obj, objTypeInfo->stringName);
            }
        }

        template <typename T, typename U>
        void local_Write(void* destination, const void* source) // #TODO Deprecate
        {
            T* objAddress = (T*)destination;

            // U* sourceObj = (U*)source;
            // *objAddress = *sourceObj;

            *objAddress = *(U*)source;
            T value = *(T*)destination;
            int bp = 0;
        }

        void local_DeserializePrimitive(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj, bool useNameString)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            void* payload = nullptr;
            if (useNameString)
            {
                payload = objJson->string;
            }
            else
            {
                payload = objJson->valuestring;
            }

            switch (objTypeInfo->enumType)
            {
            // Strings
            case MirrorTypes::m_string:
                // local_Write<std::string, const char**>(obj, objJson->valuestring); break;
                if (payload)
                {
                    std::string* str = (std::string*)obj;
                    *str = (const char*)payload;
                }
                break;
            case MirrorTypes::m_charPtr:
            case MirrorTypes::m_constCharPtr:
                // {
                //     const char* result = _strdup(objJson->valuestring);
                //     local_Write<const char*, const char*>(obj, result);
                // }
                // break;
                {
                    const char** str = (const char**)obj;
                    *str = _strdup(objJson->valuestring);
                }
                break;
            case MirrorTypes::eKeys:
            case MirrorTypes::m_char:
                local_Write<char, const char>(obj, &objJson->valuestring[0]); break;
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

            // Numbers
            case MirrorTypes::m_float:
                local_Write<float, double>(obj, &objJson->valuedouble); break;
            case MirrorTypes::m_double:
                local_Write<double, double>(obj, &objJson->valuedouble); break;
            // Bool
            case MirrorTypes::m_bool:
                local_Write<bool, bool*>(obj, &objJson->valueint); break;

            default:
                if (cJSON_Number == objJson->type)
                {
                    void* fieldAddress = ((char*)obj);
                    void* sourceAddress = (void*)&objJson->valueint;
                    memcpy(fieldAddress, sourceAddress, sizeof(objJson->valueint));
                }
                else
                {
                    // #TODO LOG_ERROR
                }
                break;
            }
        }

        void local_DeserializeClass(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            const cJSON* it = objJson->child;
            u8 index = 0;

            while (it)
            {
                // #TODO Add safety like string comparisons: objTypeInfo->name == jSON->string

                const Mirror::Field& field = objTypeInfo->fields[index];
                if (field.serializationFlags & Mirror::FieldSerializationFlags::_InspectorOnly)
                    continue;

                if (strcmp(field.name.c_str(), it->string) != 0)
                {
                    LOG_WARN("{0} Mismatched names!", __FUNCTION__);
                }
                if (field.typeInfo->isCollection())
                {
                    local_DeserializeCollection(it, field.typeInfo, (char*)obj + field.offset, field.name);
                }
                else
                {
                    DeserializeFromJson(it, field.typeInfo, (char*)obj + field.offset);
                }

                it = it->next;
                ++index;
            }
        }

        template <typename T, typename U>
        void local_DeserializeUMap(const cJSON* collectionJsonContainer, void* obj)
        {
            // if (!Mirror::InfoForType<T>()->isMap()) return;

            const Mirror::TypeInfo* mapTypeInfo = Mirror::InfoForType<std::pair<T, U>>();

            char* tempPairBuffer = new char[mapTypeInfo->size];

            const cJSON* it = collectionJsonContainer->child;
            while (it)
            {
                local_DeserializeCollection(it, mapTypeInfo, &tempPairBuffer);
                mapTypeInfo->CollectionAdd(obj, 0, &tempPairBuffer);
                it = it->next;
            }

            delete[] tempPairBuffer;
        }

        // #TODO Theoretical experiment
        void local_DeserializePair(const Mirror::TypeInfo* objTypeInfoFirst, const Mirror::TypeInfo* objTypeInfoSecond, cJSON* objJson, void* obj)
        {
            if (!objTypeInfoFirst || !objTypeInfoSecond || !obj || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }
        }

        void local_WriteNumberTemplated(void* destination, const char* source, const Mirror::TypeInfo* objTypeInfo)
        {
            switch (objTypeInfo->enumType)
            {
            // Signed
            case MirrorTypes::m_int8_t:
                {
                    s8* number = (s8*)destination;
                    *number = std::stoi(source);
                }
                break;
            case MirrorTypes::m_int16_t:
                {
                    s16* number = (s16*)destination;
                    *number = std::stoi(source);
                }
                break;
            case MirrorTypes::m_int32_t:
                {
                    s32* number = (s32*)destination;
                    *number = std::stoi(source);
                }
                break;
            case MirrorTypes::m_int64_t:
                {
                    s64* number = (s64*)destination;
                    *number = std::stoll(source);
                }
                break;
                // Unsigned
            case MirrorTypes::m_uint8_t:
                {
                    u8* number = (u8*)destination;
                    *number = std::stoi(source);
                }
                break;
            case MirrorTypes::m_uint16_t:
                {
                    u16* number = (u16*)destination;
                    *number = std::stoi(source);
                }
                break;
            case MirrorTypes::m_uint32_t:
                {
                    u32* number = (u32*)destination;
                    *number = std::stoull(source);
                }
                break;
            case MirrorTypes::m_uint64_t:
                {
                    u64* number = (u64*)destination;
                    *number = std::stoull(source);
                }
                break;

            default:
                LOG_WARN("{0} Unsupported type {1}!", __FUNCTION__, (int)objTypeInfo->enumType)
                break;
            }
        }

        template <typename T>
        void local_DeserializePair(const cJSON* objJson, void* obj)
        {
            // if (!Mirror::InfoForType<T>()->isPair()) return;

            if (!obj || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            T* objPair = (T*)obj;
            const cJSON* pairJson = objJson->child;

            // std::pair<eScriptTypes, Scriptable*>>(objJson, obj);
            // std::pair<GUID, std::string>;
            // std::pair<std::string, int32_t>;

            const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();
            const Mirror::TypeInfo* const firstTypeInfo = typeInfo->CollectionTypeInfoFirst();
            const Mirror::TypeInfo* const secondTypeInfo = typeInfo->CollectionTypeInfoSecond();

            // #NOTE Cast is for enums. This will fail for all non-numeric first types (ex. string)
            if (MirrorTypes::m_string == firstTypeInfo->enumType)
            {
                local_WriteStringTemplated<std::string, const char*>(&objPair->first, pairJson->string);
            }
            else if (MirrorTypes::m_constCharPtr == firstTypeInfo->enumType)
            {
                local_WriteStringTemplated<const char*, const char*>(&objPair->first, _strdup(pairJson->string));
            }
            else
            {
                local_WriteNumberTemplated(&objPair->first, pairJson->string, firstTypeInfo);
            }

            // Second
            if (secondTypeInfo->isPrimitive())
            {
                local_DeserializePrimitive(pairJson, secondTypeInfo, &objPair->second);
            }
            else if (secondTypeInfo->isClass)
            {
                local_DeserializeClass(pairJson, secondTypeInfo, &objPair->second);
            }
            else if (secondTypeInfo->isCollection())
            {
                local_DeserializeCollection(pairJson, secondTypeInfo, &objPair->second);
            }
            else if (secondTypeInfo->isPointer)
            {
                // #TODO Deduce absolute type
                int bp = 0;
            }
            else
            {
                LOG_WARN("{0} Unable to deduce type", __FUNCTION__);
            }
        }

        void local_DeserializeCollection(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            const Mirror::TypeInfo* const elementFirstTypeInfo = objTypeInfo->CollectionTypeInfoFirst();
            char* elementFirstBuffer = new char[elementFirstTypeInfo->size];

            const cJSON* it = objJson->child;
            size_t index = 0;

            while (it)
            {
                if (elementFirstTypeInfo->isClass ||
                    !elementFirstTypeInfo->isPrimitive() ||
                    MirrorTypes::m_string == elementFirstTypeInfo->enumType)
                {
                    // #TODO Construct before or after buffer data deserialization? Add m_DelayConstruction bool?
                    ASSERT(elementFirstTypeInfo->typeConstructorFunc, "Construct function is null!");
                    elementFirstTypeInfo->typeConstructorFunc(elementFirstBuffer);
                }

                if (objTypeInfo->isMap())
                {
                    std::unordered_map<std::string, int32_t>* mapPtr = (std::unordered_map<std::string, int32_t>*)obj;
                    if (elementFirstTypeInfo && elementFirstTypeInfo->CollectionTypeInfoFirst()->enumType == MirrorTypes::m_string)
                    {
                        // #TODO Check if construction needed
                        new (elementFirstBuffer) std::string("In");
                    }
                }

                if (objTypeInfo->isPair())
                {
                    const Mirror::TypeInfo* const elementSecondInfo = objTypeInfo->CollectionTypeInfoSecond();
                    char* elementSecondBuffer = new char[elementSecondInfo->size];

                    if (elementFirstTypeInfo->isPrimitive())
                    {
                        if (MirrorTypes::m_string == elementFirstTypeInfo->enumType)
                        {
                            // #TODO Check if construction needed
                            new (elementFirstBuffer) std::string(it->string);
                        }
                        local_DeserializePrimitive(it, elementFirstTypeInfo, elementFirstBuffer, true);
                    }
                    else
                    {
                        LOG_ERROR("{0} Non-primitive currently unsupported!", __FUNCTION__);
                        DeserializeFromJson(it, elementFirstTypeInfo, elementFirstBuffer);
                    }

                    DeserializeFromJson(it, elementSecondInfo, elementSecondBuffer);
                    objTypeInfo->CollectionAdd(obj, index, elementFirstBuffer, elementSecondBuffer); // #TODO CollectionAppend

                    delete[] elementSecondBuffer;
                }
                else
                {
                    std::vector<char>* chars = (std::vector<char>*)obj;
                    DeserializeFromJson(it, elementFirstTypeInfo, elementFirstBuffer);
                    objTypeInfo->CollectionAdd(obj, index, elementFirstBuffer); // #TODO CollectionAppend
                }

                if (elementFirstTypeInfo->isClass) // #TODO Construct before or after? Add m_DelayConstruction bool?
                {
                    // elementFirstTypeInfo->typeConstructorFunc(elementFirstBuffer);
                }

                it = it->next;
                ++index;
            }

            delete[] elementFirstBuffer;
        }

    }

}
