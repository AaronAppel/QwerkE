#include "QF_Serialization.h"

#include <unordered_map>
#include <string>

#ifdef _QDEARIMGUI
#include "Libraries/imgui/imgui.h"
#endif

#include "QC_Guid.h"

#include "QF_ScriptHelpers.h"

#include "QF_ComponentHelpers.h"

// Editor types
#include "../Editor/Source/QE_EditorWindowHelpers.h"

namespace QwerkE {

    namespace Serialization {

        cJSON* ParseJsonFile(const char* absoluteFilePath)
        {
            if (Buffer jsonFileBuffer = Files::LoadFile(absoluteFilePath))
            {
                if (cJSON* root = cJSON_Parse(jsonFileBuffer.As<char>()))
                {
                    return root;
                }
                // LOG_ERROR("{0} Could not parse JSON file {1}!", __FUNCTION__, absoluteFilePath);
                printf("%s(): Could not open cJSON stream. Possible compile error. Check %s file for typos!", __FUNCTION__, absoluteFilePath);
                return nullptr;
            }
            // LOG_ERROR("{0} Could not parse JSON file {1}!", __FUNCTION__, absoluteFilePath);
            printf("{%s}(): Could not find JSON file \"%s\"!", __FUNCTION__, absoluteFilePath);
            return nullptr;
        }

        void local_SerializePrimitive(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name);
        void local_SerializeClass(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson);
        void local_SerializeCollection(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson);

        cJSON* TestCreateObject(const char* key = nullptr);

        template<typename... Component> // #TODO Consider moving to QF_ComponentHelpers.h
        void NewSerializeComponent(EntityHandle& handle, cJSON* entityComponentsJsonArray);
        template<typename... Component>
        static void NewSerializeComponents(TemplateArgumentList<Component...>, EntityHandle& handle, cJSON* entityComponentsJsonArray);

        template <typename T, typename U>
        void TestSerializeUMap(const void* obj, cJSON* collectionJsonContainer);
        template <typename T>
        void TestSerializeVector(const void* obj, cJSON* collectionJsonContainer);
        template <typename T, size_t size>
        void TestSerializeArray(const void* obj, cJSON* collectionJsonContainer);
        template <typename T>
        void TestSerializePair(const void* obj, cJSON* collectionJsonContainer);

        template <typename T>
        cJSON* TestCreateBool(const std::string& name, const void* obj);
        template <typename T>
        cJSON* TestCreateNumber(const std::string& name, const void* obj);
        template <typename T>
        cJSON* TestCreateString(const std::string& name, const void* obj);

        void SerializeToJson(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (objTypeInfo->isPointer)
            {
                // #TODO Look at using objTypeInfo->AbsoluteType()
                LOG_ERROR("{0} Pointer types are not currently supported!", __FUNCTION__);
                return;
            }

            if (objTypeInfo->isPrimitive())
            {
                const std::string name = objTypeInfo->stringName;
                local_SerializePrimitive(obj, objTypeInfo, objJson, name);
            }
            else if (objTypeInfo->newIsClass())
            {
                cJSON* container = objJson;

                // #TODO Don't add object for super class, but still need to add it for non-abstract base classes
                if (!objTypeInfo->isAbstract && !objTypeInfo->hasSubClass())
                {
                    cJSON* classJson = TestCreateObject(objTypeInfo->stringName.c_str());
                    AddItemToObject(objJson, classJson);
                    container = classJson;
                }

                local_SerializeClass(obj, objTypeInfo, container);
            }
            else if (objTypeInfo->newIsCollection())
            {
                local_SerializeCollection(obj, objTypeInfo, objJson);
            }
            else
            {
                LOG_ERROR("{0} Error deducing type!", __FUNCTION__);
            }
        }

        void local_SerializePrimitive(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            // #TODO Actually serialize a given primitive value.

            // #TODO Try using memcpy or simple copying or raw byte data, if possible, to avoid handlling individual types.
            // Use type offset and size info.

            // memcpy_s(dest, destSize, src, srcSize);

            const size_t sizeOfType = objTypeInfo->size;

            // #TODO Consider creating an empty item here, instead of in each enum case.
            // That would mean special logic like setting the cJSON type would be needed below,
            // or more switch/if logic here (later probably better).
            cJSON* cJsonItem = nullptr;

            if (const bool testing = false)
            {
                if (MirrorTypes::m_bool == objTypeInfo->enumType)
                {
                    const bool result = *(bool*)obj;
                    cJsonItem = CreateBool(name.c_str(), result); // #TODO Deprecate QC_cJson library
                }
                else if (8 > sizeOfType) // 2 or 4 bytes
                {
                    cJsonItem = CreateNumber(name.c_str(), 0.);
                    memcpy((void*)&cJsonItem->valueint, obj, sizeOfType); // #TODO Pick one value to use
                    memcpy((void*)&cJsonItem->valuedouble, obj, sizeOfType);
                }
                else if (8 == sizeOfType)
                {
                    cJsonItem = CreateNumber(name.c_str(), 0.);
                    memcpy((void*)&cJsonItem->valuedouble, obj, sizeOfType);
                }
                else
                {
                    // Write to string?

                    // uint64_t* numberAddress = (uint64_t*)obj;
                    // AddItemToArray(objJson, CreateString(name.c_str(), std::to_string(*numberAddress).c_str()));
                }
            }

            switch (objTypeInfo->enumType)
            {
            case MirrorTypes::m_string:
                {
                    const std::string* fieldAddress = (std::string*)obj;
                    cJsonItem = CreateString(name.c_str(), fieldAddress->data()); // #TODO Requires ->data() so can't work with TestCreateString()
                }
                break;

            case MirrorTypes::eKeys:
            case MirrorTypes::m_char: // #TODO Note that const is forced here
            case MirrorTypes::m_charPtr:
            case MirrorTypes::m_constCharPtr:
                cJsonItem = TestCreateString<const char*>(name, obj); break;
            case MirrorTypes::m_bool:
                cJsonItem = TestCreateBool<bool*>(name, obj); break;
            case MirrorTypes::EditorWindowFlags:
            case MirrorTypes::EditorWindowTypes:
            case MirrorTypes::eScriptTypes: // #TODO Add a case for all enums by default
            case MirrorTypes::m_eSceneTypes:
            case MirrorTypes::m_uint8_t:
                cJsonItem = TestCreateNumber<uint8_t*>(name, obj); break;
            case MirrorTypes::m_uint16_t:
                cJsonItem = TestCreateNumber<uint16_t*>(name, obj); break;
            case MirrorTypes::m_uint32_t:
                cJsonItem = TestCreateNumber<uint32_t*>(name, obj); break;
            case MirrorTypes::m_int8_t:
                cJsonItem = TestCreateNumber<int8_t*>(name, obj); break;
            case MirrorTypes::m_int16_t:
                cJsonItem = TestCreateNumber<int16_t*>(name, obj); break;
            case MirrorTypes::m_int:
            case MirrorTypes::m_int32_t:
                cJsonItem = TestCreateNumber<int32_t*>(name, obj); break;
            case MirrorTypes::m_int64_t:
                cJsonItem = TestCreateNumber<int64_t*>(name, obj); break;

            case MirrorTypes::m_uint64_t: // #NOTE Special case of conversion on 64 bit types
                {
                    // Use string instead of a double to avoid conversion issues
                    uint64_t* numberAddress = (uint64_t*)obj;
                    cJsonItem = CreateString(name.c_str(), std::to_string(*numberAddress).c_str());
                }
                // cJsonItem = TestCreateNumber<uint64_t*>(name, obj);
                break;

            case MirrorTypes::m_float:
                cJsonItem = TestCreateNumber<float*>(name, obj); break;
            case MirrorTypes::m_double:
                cJsonItem = TestCreateNumber<double*>(name, obj); break;

            default:
                LOG_ERROR("{0} Unsupported user defined field type {1} {2}({3}) for serialization!", __FUNCTION__, name, objTypeInfo->stringName, (int)objTypeInfo->enumType);
                break;
            }

            if (cJsonItem)
            {
                AddItemToObject(objJson, cJsonItem);
            }
        }

        void local_SerializeClass(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (!objTypeInfo->isClass)
            {
                LOG_ERROR("{0} Invalid type info!", __FUNCTION__);
                return;
            }

            if (objTypeInfo->isSubClass())
            {
                if (objTypeInfo->superTypeInfo)
                {
                    SerializeToJson(obj, objTypeInfo->superTypeInfo, objJson);
                }
                else
                {
                    LOG_ERROR("{0} Invalid type info!", __FUNCTION__);
                    return;
                }
            }

            if (objTypeInfo->fields.empty())
            {
                LOG_WARN("{0} Class has no serialized members", __FUNCTION__);
            }

            const Editor::EditorWindow* potentialWindow = (Editor::EditorWindow*)obj;

            for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
            {
                const Mirror::Field& field = objTypeInfo->fields[i];

                if (field.serializationFlags & Mirror::FieldSerializationFlags::_InspectorOnly)
                    continue;

                void* fieldAddress = (char*)obj + field.offset;

                if (field.typeInfo->isPrimitive()) // #TODO Review handling primitive here (dependent on field name)
                {
                    const std::string name = field.name;
                    const std::string* potentialField = (std::string*)fieldAddress;
                    local_SerializePrimitive(fieldAddress, field.typeInfo, objJson, name);
                }
                else
                {
                    // #TODO This also checks if primitive so maybe avoid redundancy
                    SerializeToJson(fieldAddress, field.typeInfo, objJson);
                }
            }
        }

        void local_SerializeCollection(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (false) // (!objTypeInfo->newIsCollection()) // || !objTypeInfo->collectionTypeInfo)
            {
                LOG_ERROR("{0} Invalid type info!", __FUNCTION__);
                return;
            }

            cJSON* collectionJsonContainer = TestCreateObject(objTypeInfo->stringName.c_str());
            AddItemToObject(objJson, collectionJsonContainer);

            switch (objTypeInfo->enumType)
            {
            case MirrorTypes::m_map_guid_entt:
                TestSerializeUMap<GUID, entt::entity>(obj, collectionJsonContainer); break;
            case MirrorTypes::m_map_eScriptTypes_ScriptablePtr:
                TestSerializeUMap<eScriptTypes, Scriptable*>(obj, collectionJsonContainer); break;
            case MirrorTypes::m_umap_guid_editorWindowPtr:
                TestSerializeUMap<GUID, Editor::EditorWindow*>(obj, collectionJsonContainer); break;

            case MirrorTypes::m_vec_string:
                TestSerializeVector<std::string>(obj, collectionJsonContainer); break;
            case MirrorTypes::m_vec_pair_guid_string:
                TestSerializeVector<std::pair<GUID, std::string>>(obj, collectionJsonContainer); break;

            case MirrorTypes::m_imvec4_array: // imgui types
                TestSerializeArray<ImVec4, ImGuiCol_COUNT>(obj, collectionJsonContainer); break;
            case MirrorTypes::m_arr_float16:
                TestSerializeArray<float, 16>(obj, collectionJsonContainer); break;

            case MirrorTypes::m_pair_eScriptTypes_ScriptablePtr:
                TestSerializePair<std::pair<eScriptTypes, Scriptable*>>(obj, collectionJsonContainer); break;
            case MirrorTypes::m_pair_guid_string:
                // #TODO Make type agnostic for other types of pairs
                // typedef std::pair<GUID, std::string> m_pair_guid_string;
                // MIRROR_PAIR(m_pair_guid_string, GUID, std::string);
                using PairGuidString = std::pair<GUID, std::string>;
                TestSerializePair<PairGuidString>(obj, collectionJsonContainer); break;
            case MirrorTypes::m_pair_guid_editorWindowPtr:
                TestSerializePair<std::pair<GUID, Editor::EditorWindow*>>(obj, collectionJsonContainer); break;

            default:
                LOG_WARN("{0} Unsupported user defined field type {1}({2}) for serialization!", __FUNCTION__, objTypeInfo->stringName, (u32)objTypeInfo->enumType);
                break;
            }
        }
        cJSON* TestCreateObject(const char* key)
        {
            cJSON* returnJSONItem = cJSON_CreateObject();
            if (key) returnJSONItem->string = _strdup(key); // #TODO Look into deallocation responsibility
            return returnJSONItem;
        }

        template<typename... Component> // #TODO Consider moving to QF_ComponentHelpers.h
        void NewSerializeComponent(EntityHandle& handle, cJSON* entityComponentsJsonArray)
        {
            ([&]()
            {
                if (!handle.HasComponent<Component>())
                    return;

                const Mirror::TypeInfo* componentTypeInfo = Mirror::InfoForType<Component>();

                cJSON* componentJsonItem = TestCreateObject(componentTypeInfo->stringName.c_str());
                AddItemToObject(entityComponentsJsonArray, componentJsonItem);

                Component& component = handle.GetComponent<Component>();
                local_SerializeClass(&component, componentTypeInfo, componentJsonItem);
            }(), ...);
        }

        template<typename... Component>
        static void NewSerializeComponents(TemplateArgumentList<Component...>, EntityHandle& handle, cJSON* entityComponentsJsonArray)
        {
            NewSerializeComponent<Component...>(handle, entityComponentsJsonArray);
        }

        template <typename T, typename U>
        void TestSerializeUMap(const void* obj, cJSON* collectionJsonContainer)
        {
            // if (!Mirror::InfoForType<T>()->isMap()) return;

            const std::unordered_map<T, U>* scriptsMap = (std::unordered_map<T, U>*)obj;

            const Mirror::TypeInfo* objTypeInfo = Mirror::InfoForType<std::unordered_map<T, U>>();

            if (MirrorTypes::m_map_guid_entt == objTypeInfo->enumType)
            {
                // #NOTE m_map_guid_entt type should only be owned by a Scene.
                // Look at enforcing that through some static asserts of runtime cast checks
                const std::unordered_map<GUID, entt::entity>* entitiesMap = (std::unordered_map<GUID, entt::entity>*)obj;

                const Mirror::TypeInfo* sceneInfo = Mirror::InfoForType<Scene>();
                if (!sceneInfo || sceneInfo->enumType != MirrorTypes::Scene)
                {
                    LOG_ERROR("{0} Cannot serialize {1} type!", __FUNCTION__, objTypeInfo->stringName.c_str());
                    return;
                }

                const std::string memberName = "m_GuidsToEntts";
                size_t m_GuidsToEntts_Offset = INT8_MAX;
                for (size_t i = 0; i < sceneInfo->fields.size(); i++)
                {
                    const Mirror::Field& field = sceneInfo->fields[i];
                    if (field.name == memberName)
                    {
                        m_GuidsToEntts_Offset = field.offset;
                        break;
                    }
                }

                if (INT8_MAX == m_GuidsToEntts_Offset)
                {
                    LOG_CRITICAL("{0} Could not serialize type {1} {2}!", __FUNCTION__, objTypeInfo->stringName, (u8)objTypeInfo->enumType);
                    return;
                }

                // #TODO Fix ugly special case requiring knowledge of owning type.
                // Look at adding some objTypeInfo->Owner that returns the start of the owning type object instance in memory
                Scene* scene = (Scene*)((char*)obj - m_GuidsToEntts_Offset);

                for (auto& entityPair : *entitiesMap)
                {
                    EntityHandle handle(scene, entityPair.second);

                    // #TODO Special case where GUID needs to be known before object creation,
                    // So create an array with the GUID as the name. Review for better solution.
                    cJSON* entityJsonContainer = TestCreateObject(std::to_string(handle.EntityGuid()).c_str());
                    AddItemToObject(collectionJsonContainer, entityJsonContainer);
                    NewSerializeComponents(EntityComponentsList{}, handle, entityJsonContainer);
                }
                return;
            }

            for (auto& scriptTypeScriptablePair : *scriptsMap)
            {
                if (!objTypeInfo->CollectionTypeInfoFirst())
                {
                    LOG_ERROR("{0} Null collection type!", __FUNCTION__);
                    break;
                }

                SerializeToJson((void*)&scriptTypeScriptablePair, objTypeInfo->CollectionTypeInfoFirst(), collectionJsonContainer);
            }
        }

        template <typename T>
        void TestSerializeVector(const void* obj, cJSON* collectionJsonContainer)
        {
            // if (!Mirror::InfoForType<T>()->isVector()) return;

            const std::vector<T>* objVector = (std::vector<T>*)obj;

            for (size_t i = 0; i < objVector->size(); i++)
            {
                const T& vectorElement = objVector->at(i);

                const Mirror::TypeInfo* objTypeInfo = Mirror::InfoForType<T>();
                if (objTypeInfo->isPointer) LOG_WARN("{0} Review pointer type usage", __FUNCTION__)
                SerializeToJson((void*)&vectorElement, objTypeInfo, collectionJsonContainer);
            }
        }

        template <typename T, size_t size>
        void TestSerializeArray(const void* obj, cJSON* collectionJsonContainer)
        {
            // if (!Mirror::InfoForType<T>()->isArray()) return;

            const T* objArray = (T*)obj;
            for (size_t i = 0; i < size; i++)
            {
                const Mirror::TypeInfo* objTypeInfo = Mirror::InfoForType<T>();
                if (objTypeInfo->isPointer) LOG_WARN("{0} Review pointer type usage", __FUNCTION__)
                SerializeToJson((void*)&objArray[i], objTypeInfo, collectionJsonContainer);
            }
        }

        template <typename T>
        void TestSerializePair(const void* obj, cJSON* collectionJsonContainer)
        {
            // if (!Mirror::InfoForType<T>()->isPair()) return;

            const Mirror::TypeInfo* const objTypeInfo = Mirror::InfoForType<T>();;
            const Mirror::TypeInfo* const firstTypeInfo = objTypeInfo->CollectionTypeInfoFirst();
            const Mirror::TypeInfo* const absoluteFirstTypeInfo = firstTypeInfo->AbsoluteType();
            const Mirror::TypeInfo* const secondTypeInfo = objTypeInfo->CollectionTypeInfoSecond();
            const Mirror::TypeInfo* const absoluteSecondTypeInfo = secondTypeInfo->AbsoluteType();

            const T* objPair = (T*)obj;

            if (objTypeInfo->CollectionTypeInfoFirst()->isPointer) LOG_WARN("{0} Review first pointer type usage", __FUNCTION__)
            // if (objTypeInfo->CollectionTypeInfoSecond()->isPointer) LOG_WARN("{0} Review second pointer type usage", __FUNCTION__)

            if (!firstTypeInfo->isPrimitive() && MirrorTypes::GUID != firstTypeInfo->enumType)
            {
                // #NOTE The first type is assumed to be primitive
                LOG_WARN("{0} Verify that non-primitive pair.first is valid and works with current logic", __FUNCTION__);
            }

            void* absoluteObjectAddress = (void*)&objPair->second;
            if (secondTypeInfo->isPointer) // #TODO Handle multiple levels of pointers
            {
                absoluteObjectAddress = *(void**)absoluteObjectAddress;
            }

            if (absoluteSecondTypeInfo->isPrimitive())
            {
                local_SerializePrimitive(absoluteObjectAddress, absoluteSecondTypeInfo, collectionJsonContainer, std::to_string(objPair->first));
            }
            else if (absoluteSecondTypeInfo->isClass)
            {
                cJSON* classJsonItem = TestCreateObject(std::to_string(objPair->first).c_str());
                AddItemToObject(collectionJsonContainer, classJsonItem);

                if (absoluteSecondTypeInfo->hasSubClass())
                {
                    u8 derivedType = UINT8_MAX;

                    switch (objTypeInfo->enumType)
                    {
                    case MirrorTypes::m_pair_guid_editorWindowPtr:
                        {
                            using PairType = std::pair<GUID, Editor::EditorWindow*>;
                            const PairType* p = (PairType*)obj;
                            derivedType = p->second->Type();
                        }
                        break;

                    case MirrorTypes::m_pair_eScriptTypes_ScriptablePtr:
                        {
                            using PairType = std::pair<eScriptTypes, Scriptable*>;
                            const PairType* p = (PairType*)obj;
                            derivedType = p->second->ScriptType();
                        }
                        break;

                    default:
                        LOG_WARN("{0} Unsupported pair type", __FUNCTION__); break;
                    }

                    if (const Mirror::TypeInfo* derivedTypeInfo = absoluteSecondTypeInfo->DerivedTypeInfo(derivedType))
                    {
                        local_SerializeClass(absoluteObjectAddress, derivedTypeInfo, classJsonItem);
                        return;
                    }
                }

                if (absoluteSecondTypeInfo->isAbstract)
                {
                    LOG_ERROR("{0} Could not deduce derived type!", __FUNCTION__);
                    return;
                }
                local_SerializeClass(absoluteObjectAddress, absoluteSecondTypeInfo, classJsonItem);
            }
            else if (absoluteSecondTypeInfo->isCollection())
            {
                cJSON* collectionJsonItem = TestCreateObject();
                local_SerializeCollection(absoluteObjectAddress, absoluteSecondTypeInfo, collectionJsonItem);
            }
        }

        template <typename T>
        cJSON* TestCreateBool(const std::string& name, const void* obj) // #TODO Rename from "Test"
        {
            return CreateBool(name.c_str(), *(T)obj);
        }

        template <typename T>
        cJSON* TestCreateNumber(const std::string& name, const void* obj) // #TODO Rename from "Test"
        {
            return CreateNumber(name.c_str(), *(T)obj);
        }

        template <typename T>
        cJSON* TestCreateString(const std::string& name, const void* obj) // #TODO Rename from "Test"
        {
            return CreateString(name.c_str(), (T)obj); // #NOTE obj is not dereferenced
        }

    }

}
