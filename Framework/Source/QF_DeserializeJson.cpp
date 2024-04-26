#include "QF_Serialization.h"

#include "QF_ScriptHelpers.h"

#include "QF_ComponentHelpers.h"

// Editor types
#include "../Editor/Source/QE_EditorWindowHelpers.h"

namespace QwerkE {

    namespace Serialization {

        void local_DeserializePrimitive(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj);
        void local_DeserializeClass(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj);
        void local_DeserializeCollection(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj);

        void DeserializeFromJson(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            // m_vec_pair_guid_string

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
                local_DeserializeCollection(objJson, objTypeInfo, obj);
            }
        }

        template <typename T, typename U>
        void local_Write(void* destination, const void* source)
        {
            T* objAddress = (T*)destination;
            *objAddress = *(U*)source;
            T value = *(T*)destination;
            int bp = 0;
        }

        void local_DeserializePrimitive(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            switch (objTypeInfo->enumType)
            {
            // Strings
            case MirrorTypes::m_string:
                // Write<std::string, const char*>(obj, objJson->valuestring); break;
                {
                    std::string* str = (std::string*)obj;
                    *str = objJson->valuestring;
                }
                break;
            case MirrorTypes::m_charPtr:
            case MirrorTypes::m_constCharPtr:
                {
                    const char** str = (const char**)obj;
                    *str = _strdup(objJson->valuestring);
                }
                break;
                // Write<char*, char*>(obj, _strdup(objJson->valuestring)); break;
            case MirrorTypes::eKeys:
            case MirrorTypes::m_char:
                // Write<char*, char*>(obj, _strdup(objJson->valuestring[0])); break;
                {
                    char* fieldAddress = (char*)obj;
                    *fieldAddress = objJson->valuestring[0];
                }
                break;

            case MirrorTypes::m_uint64_t: // #NOTE Storing uint64 as string
                {
                    u64* valuePtr = (u64*)obj;
                    const u64 temp = std::stoull(objJson->valuestring);
                    *valuePtr = temp;
                }
                break;
            case MirrorTypes::m_int64_t: // #NOTE Storing int64 as string
                {
                    s64* valuePtr = (s64*)obj;
                    const s64 temp = std::stoll(objJson->valuestring);
                    *valuePtr = temp;
                }
                break;

            // Numbers
            case MirrorTypes::m_float:
                local_Write<float, double>(obj, &objJson->valuedouble); break;
            case MirrorTypes::m_double:
                local_Write<double, double>(obj, &objJson->valuedouble); break;
            // Bool
            case MirrorTypes::m_bool:
                {
                    bool* fieldAddress = ((bool*)obj);
                    *fieldAddress = objJson->type == cJSON_True;
                }
                break;

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
                const Mirror::Field& field = objTypeInfo->fields[index];
                DeserializeFromJson(it, field.typeInfo, (char*)obj + field.offset);
                it = it->next;
                ++index;
            }

            // for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
            {
            }
        }

        template <typename T, typename U>
        void local_DeserializeUMap(const cJSON* collectionJsonContainer, void* obj)
        {
            // if (!Mirror::InfoForType<T>()->isMap()) return;

            // TestDeserializeUMap<GUID, entt::entity>(objJson, obj); break;
            // TestDeserializeUMap<eScriptTypes, Scriptable*>(objJson, obj); break;
            // TestDeserializeUMap<GUID, Editor::EditorWindow*>(objJson, obj); break;
            // TestDeserializeUMap<std::string, s32>(objJson, obj); break;

            std::unordered_map<T, U>* objMapOfPairs = (std::unordered_map<T, U>*)obj;
            objMapOfPairs->clear();

            const Mirror::TypeInfo* mapTypeInfo = Mirror::InfoForType<std::pair<T, U>>();

            const cJSON* it = collectionJsonContainer->child;
            while (it)
            {
                std::pair<T, U> tempPair;
                local_DeserializeCollection(it, mapTypeInfo, &tempPair);
                objMapOfPairs->insert(tempPair);
                it = it->next;
            }
        }

        template <typename T>
        void local_DeserializeVector(const cJSON* collectionJsonContainer, void* obj)
        {
            // if (!Mirror::InfoForType<T>()->isVector()) return;

            std::vector<T>* objVector = (std::vector<T>*)obj;
            // objVector->clear();

            const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();

            const cJSON* it = collectionJsonContainer->child;
            while (it)
            {
                T newT;
                DeserializeFromJson(it, typeInfo, &newT);
                objVector->push_back(newT);
                it = it->next;
            }
        }

        template <typename T, size_t size>
        void local_DeserializeArray(const cJSON* collectionJsonContainer, void* obj)
        {
            // if (!Mirror::InfoForType<T>()->isArray()) return;
            T* objArray = (T*)obj;

            const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();

            const cJSON* it = collectionJsonContainer->child;
            size_t index = 0;
            while (it && index < size)
            {
                T newT;
                DeserializeFromJson(it, typeInfo, &newT);
                objArray[index] = newT;
                it = it->next;
                ++index;
            }
        }

        template <typename T, typename Parent>
        void local_DeserializeContiguous(const cJSON* collectionJsonContainer, void* obj)
        {
            // #TODO Try to remove template requirement with
            // objTypeInfo->collectionTypeInfo()
            // (OPTIONAL) and a typeInfo->count for number of elements
            const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();
            const Mirror::TypeInfo* ParentTypeInfo = Mirror::InfoForType<Parent>();

            std::vector<char[2]> vec2; // #TODO See if a dynamic size can be used

            if (ParentTypeInfo->isVector())
            {
                // #TODO Could look at removing template type using :
                // std::vector<(sized member)> vec; // Could be an array or dynamic array type?
                // vec.reserve(element count);
                // for () {
                //     temp;
                //     deserialize(temp);
                //     vec[i] = temp;
                // }
                // vec.resize(element count);
            }

            const cJSON* it = collectionJsonContainer->child;
            size_t index = 0;

            std::vector<T>* vec = (std::vector<T>*)obj; // #TODO T required

            while (it)
            {
                T newT;
                DeserializeFromJson(it, typeInfo, &newT);
                if (ParentTypeInfo->isVector())
                {
                    vec->push_back(newT);
                }
                else
                {
                    memcpy((char*)obj + (sizeof(T) * index), &newT, sizeof(T));
                }
                it = it->next;
                ++index;
            }

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

        template <typename T, typename U>
        void local_WriteStringTemplated(void* destination, const void* source)
        {
            if (!source || !destination)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            T* destinationAddress = (T*)destination;
            *destinationAddress = (U)source;
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

        void local_DeserializeCollection(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            // #TODO Try this instead of switch
            // TestDeserializePair(objTypeInfo->CollectionTypeInfoFirst(), objTypeInfo->CollectionTypeInfoSecond(), objJson, obj); break;
            // Might need "hard" types for iteration

            switch (objTypeInfo->enumType)
            {
            case MirrorTypes::m_map_guid_entt:
                local_DeserializeUMap<GUID, entt::entity>(objJson, obj); break;
            case MirrorTypes::m_map_eScriptTypes_ScriptablePtr:
                local_DeserializeUMap<eScriptTypes, Scriptable*>(objJson, obj); break;
            case MirrorTypes::m_umap_guid_editorWindowPtr:
                local_DeserializeUMap<GUID, Editor::EditorWindow*>(objJson, obj); break;
            case MirrorTypes::m_umap_string_int32:
                local_DeserializeUMap<std::string, s32>(objJson, obj); break;

            case MirrorTypes::m_vec_string:
                local_DeserializeVector<std::string>(objJson, obj); break;
            case MirrorTypes::m_vec_pair_guid_string:
                local_DeserializeVector<std::pair<GUID, std::string>>(objJson, obj); break;
            case MirrorTypes::m_vec_char:
                // TestDeserializeVector<char>(objJson, obj); break;
                local_DeserializeContiguous<char, std::vector<char>>(objJson, obj); break;

            case MirrorTypes::m_imvec4_array: // imgui types
                // TestDeserializeArray<ImVec4, ImGuiCol_COUNT>(objJson, obj); break;
                local_DeserializeContiguous<ImVec4, ImVec4*>(objJson, obj); break;
            case MirrorTypes::m_arr_float16:
                // TestDeserializeArray<float, 16>(objJson, obj); break;
            case MirrorTypes::m_arr_float10:
                // TestDeserializeArray<float, 10>(objJson, obj); break;
                local_DeserializeContiguous<float, float[10]>(objJson, obj); break;

            case MirrorTypes::m_pair_eScriptTypes_ScriptablePtr:
                local_DeserializePair<std::pair<eScriptTypes, Scriptable*>>(objJson, obj); break;
            case MirrorTypes::m_pair_guid_string:
                // #TODO Make type agnostic for other types of pairs
                // typedef std::pair<GUID, std::string> m_pair_guid_string;
                // MIRROR_PAIR(m_pair_guid_string, GUID, std::string);
                using PairGuidString = std::pair<GUID, std::string>;
                local_DeserializePair<PairGuidString>(objJson, obj); break;
            case MirrorTypes::m_pair_guid_editorWindowPtr:
                local_DeserializePair<std::pair<GUID, Editor::EditorWindow*>>(objJson, obj); break;
            case MirrorTypes::m_pair_string_int32:
                local_DeserializePair<std::pair<std::string, s32>>(objJson, obj); break;

            default:
                break;
            }
        }

    }

}
