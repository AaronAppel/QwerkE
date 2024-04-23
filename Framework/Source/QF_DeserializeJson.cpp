#include "QF_Serialization.h"

#include "QF_ScriptHelpers.h"

#include "QF_ComponentHelpers.h"

// Editor types
#include "../Editor/Source/QE_EditorWindowHelpers.h"

namespace QwerkE {

    namespace Serialization {

        void local_DeserializePrimitive(cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj);
        void local_DeserializeClass(cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj);
        void local_DeserializeCollection(cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj);

        void DeserializeFromJson(cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            // m_vec_pair_guid_string

            if (objTypeInfo->isPrimitive())
            {
                local_DeserializePrimitive(objJson, objTypeInfo, obj);
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

        void local_DeserializePrimitive(cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }
        }

        void local_DeserializeClass(cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }
        }

        template <typename T, typename U>
        void TestDeserializeUMap(cJSON* collectionJsonContainer, void* obj)
        {
            // if (!Mirror::InfoForType<T>()->isMap()) return;

            std::pair<T, U>* objMapOfPairs = (std::pair<T, U>*)obj;

            cJSON* it = collectionJsonContainer;
            while (it)
            {
                std::pair<T, U> tempPair;
                if (std::is_pointer_v<U>)
                {
                    // tempPair.second = new U();
                }
                // objMapOfPairs->insert()
                it = it->next;
            }
        }

        template <typename T>
        void TestDeserializeVector(cJSON* collectionJsonContainer, void* obj)
        {
            // if (!Mirror::InfoForType<T>()->isVector()) return;
        }

        template <typename T, size_t size>
        void TestDeserializeArray(cJSON* collectionJsonContainer, void* obj)
        {
            // if (!Mirror::InfoForType<T>()->isArray()) return;
        }

        // #TODO Theoretic experiment
        void TestDeserializePair(const Mirror::TypeInfo* objTypeInfoFirst, const Mirror::TypeInfo* objTypeInfoSecond, cJSON* objJson, void* obj)
        {
            if (!objTypeInfoFirst || !objTypeInfoSecond || !obj || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }
        }

        template <typename T>
        void TestDeserializePair(cJSON* objJson, void* obj)
        {
            // if (!Mirror::InfoForType<T>()->isPair()) return;

            if (!obj || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            T* objPair = (T*)obj;

            // std::pair<eScriptTypes, Scriptable*>>(objJson, obj);
            // std::pair<GUID, std::string>;

            // if (std::is_enum_v<decltype(objPair->first)>)
            // #NOTE Cast is for enums. This will fail for all non-numeric first types (ex. string)
            objPair->first = (decltype(objPair->first))std::stoull(objJson->string);

            const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();
            const Mirror::TypeInfo* const secondTypeInfo = typeInfo->CollectionTypeInfoSecond();
            if (secondTypeInfo->isPrimitive())
            {
                local_DeserializePrimitive(objJson, secondTypeInfo, obj);
            }
            else if (secondTypeInfo->isClass)
            {
                local_DeserializeClass(objJson, secondTypeInfo, obj);
            }
            else if (secondTypeInfo->isCollection())
            {
                local_DeserializeCollection(objJson, secondTypeInfo, obj);
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

        void local_DeserializeCollection(cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj)
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
                TestDeserializeUMap<GUID, entt::entity>(objJson, obj); break;
            case MirrorTypes::m_map_eScriptTypes_ScriptablePtr:
                TestDeserializeUMap<eScriptTypes, Scriptable*>(objJson, obj); break;
            case MirrorTypes::m_umap_guid_editorWindowPtr:
                TestDeserializeUMap<GUID, Editor::EditorWindow*>(objJson, obj); break;

            case MirrorTypes::m_vec_string:
                TestDeserializeVector<std::string>(objJson, obj); break;
            case MirrorTypes::m_vec_pair_guid_string:
                TestDeserializeVector<std::pair<GUID, std::string>>(objJson, obj); break;

            case MirrorTypes::m_imvec4_array: // imgui types
                TestDeserializeArray<ImVec4, ImGuiCol_COUNT>(objJson, obj); break;
            case MirrorTypes::m_arr_float16:
                TestDeserializeArray<float, 16>(objJson, obj); break;

            case MirrorTypes::m_pair_eScriptTypes_ScriptablePtr:
                TestDeserializePair<std::pair<eScriptTypes, Scriptable*>>(objJson, obj); break;
            case MirrorTypes::m_pair_guid_string:
                // #TODO Make type agnostic for other types of pairs
                // typedef std::pair<GUID, std::string> m_pair_guid_string;
                // MIRROR_PAIR(m_pair_guid_string, GUID, std::string);
                using PairGuidString = std::pair<GUID, std::string>;
                TestDeserializePair<PairGuidString>(objJson, obj); break;
            case MirrorTypes::m_pair_guid_editorWindowPtr:
                TestDeserializePair<std::pair<GUID, Editor::EditorWindow*>>(objJson, obj); break;
            default:
                break;
            }
        }

    }

}
