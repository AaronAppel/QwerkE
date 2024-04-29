#include "QF_Serialization.h"

#include <unordered_map>
#include <string>

#ifdef _QDEARIMGUI
#include "Libraries/imgui/imgui.h"
#endif

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#include "QC_Guid.h"

#include "QF_ComponentHelpers.h"
#include "QF_EntityHandle.h"
#include "QF_Files.h"
#include "QF_Log.h"
#include "QF_Scene.h"
#include "QF_ScriptHelpers.h"

// Editor types
#include "../Editor/Source/QE_EditorWindowHelpers.h"

namespace QwerkE {

    namespace Serialization {

        // #TODO Note cJSON_AddItemToArray adds to object. Maybe create a helper for consistant readability

        cJSON* ParseJsonFile(const char* absoluteFilePath)
        {
            if (Buffer jsonFileBuffer = Files::LoadFile(absoluteFilePath))
            {
                if (cJSON* root = cJSON_Parse(jsonFileBuffer.As<char>()))
                {
                    return root;
                }
                LOG_ERROR("{0} Could not parse JSON file {1}! Possible compile error. Check file for typos", __FUNCTION__, absoluteFilePath);
                return nullptr;
            }
            LOG_ERROR("{0} Could not load JSON file {1}!", __FUNCTION__, absoluteFilePath);
            return nullptr;
        }

        // #TODO Move to a Files:: helper for writing to a file using a Buffer object
        #include <stdio.h>
        void PrintJsonToFile(cJSON* jsonRootObject, const char* absoluteFilePath)
        {
            if (jsonRootObject && absoluteFilePath)
            {
                const char* jsonStructureString = cJSON_Print(jsonRootObject);
                {
                    FILE* filehandle;
                    errno_t error = fopen_s(&filehandle, absoluteFilePath, "w+");
                    if (filehandle)
                    {
                        fwrite(jsonStructureString, 1, strlen(jsonStructureString), filehandle);
                        fclose(filehandle);
                    }
                    else
                    {
                        // #TODO Error message
                    }
                }

                free((char*)jsonStructureString);
            }
        }

        cJSON* TempCreateArray(const char* key)
        {
            cJSON* returnArray = cJSON_CreateArray();
            returnArray->child = cJSON_CreateObject();
            returnArray->string = _strdup(key);
            return returnArray;
        }

        void local_SerializePrimitive(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name);
        void local_SerializeClass(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson);
        void local_SerializeCollection(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name);

        cJSON* TestCreateObject(const char* key = nullptr);

        template<typename... Component> // #TODO Consider moving to QF_ComponentHelpers.h
        void NewSerializeComponent(EntityHandle& handle, cJSON* entityComponentsJsonArray);
        template<typename... Component>
        static void NewSerializeComponents(TemplateArgumentList<Component...>, EntityHandle& handle, cJSON* entityComponentsJsonArray);

        template <typename T>
        void TestSerializePair(const void* obj, cJSON* collectionJsonContainer);

        void TestAgnostic(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson);

        template <typename T>
        cJSON* TestCreateBool(const std::string& name, const void* obj);
        template <typename T>
        cJSON* TestCreateNumber(const std::string& name, const void* obj);
        template <typename T>
        cJSON* TestCreateString(const std::string& name, const void* obj);

        template <typename T>
        void SerializeComponent(EntityHandle& handle, cJSON* entityComponentsJsonArray)
        {
            if (!handle.HasComponent<T>())
                return;

            T& component = handle.GetComponent<T>();
            auto componentTypeInfo = Mirror::InfoForType<T>();
            cJSON* newJsonObjectArray = TestCreateObject(componentTypeInfo->stringName.c_str());
            SerializeToJson(&component, componentTypeInfo, newJsonObjectArray, componentTypeInfo->stringName);
            cJSON_AddItemToArray(entityComponentsJsonArray, newJsonObjectArray);
        }

        bool TypeInfoHasSerializeOverride(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson)
        {
            switch (objTypeInfo->enumType)
            {
            case MirrorTypes::m_map_guid_entt:
                {
                    const std::unordered_map<GUID, entt::entity>* entitiesMap = (std::unordered_map<GUID, entt::entity>*)obj;
                    Scene* scene = (Scene*)obj;

                    for (auto& entityPair : *entitiesMap)
                    {
                        EntityHandle handle(scene, entityPair.second);

                        // cJSON* entityJsonArray = CreateArray("Entity");
                        cJSON* entityJsonArray = TestCreateObject(std::to_string(handle.EntityGuid()).c_str());
                        cJSON_AddItemToArray(objJson, entityJsonArray);

                        cJSON* entityComponentsJsonArray = TestCreateObject("Components");
                        cJSON_AddItemToArray(objJson, entityComponentsJsonArray);

                        SerializeComponent<ComponentCamera>(handle, entityComponentsJsonArray);
                        SerializeComponent<ComponentInfo>(handle, entityComponentsJsonArray);
                        SerializeComponent<ComponentMesh>(handle, entityComponentsJsonArray);
                        SerializeComponent<ComponentTransform>(handle, entityComponentsJsonArray);
                        SerializeComponent<ComponentScript>(handle, entityComponentsJsonArray);
                    }
                }
                return true;
            }
            return false;
        }

        void SerializeToJson(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (TypeInfoHasSerializeOverride(obj, objTypeInfo, objJson))
                return;

            if (objTypeInfo->isPointer)
            {
                // Add object
                // #TODO Look to remove pointer as category
            }

            switch (objTypeInfo->category)
            {
            case Mirror::TypeInfoCategories::TypeInfoCategory_Primitive:
                {
                    const std::string name = objTypeInfo->stringName;
                    local_SerializePrimitive(obj, objTypeInfo, objJson, name);
                }
                break;
            case Mirror::TypeInfoCategories::TypeInfoCategory_Class:
                {
                    cJSON* classJson = TestCreateObject(name.c_str());
                    cJSON_AddItemToArray(objJson, classJson);
                    local_SerializeClass(obj, objTypeInfo, classJson);
                }
                break;
            case Mirror::TypeInfoCategories::TypeInfoCategory_Collection:
                local_SerializeCollection(obj, objTypeInfo, objJson, name); break;
            case Mirror::TypeInfoCategories::TypeInfoCategory_Pointer: // #TODO Look to remove
                {
                    if (nullptr == *(const void**)obj)
                    {
                        LOG_ERROR("{0} Pointer is null!", __FUNCTION__);
                        return;
                    }

                    cJSON* pointerJson = TestCreateObject(name.c_str());
                    cJSON_AddItemToArray(objJson, pointerJson);

                    const Mirror::TypeInfo* absoluteTypeInfo = objTypeInfo->AbsoluteType();
                    SerializeToJson(*(void**)obj, absoluteTypeInfo, pointerJson, absoluteTypeInfo->stringName);
                }
                break;

            case Mirror::TypeInfoCategories::TypeInfoCategory_Invalid:
            default:
                ASSERT(false, "Unsupported category!");
                break;
            }
        }

        void local_SerializePrimitive(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            // #TODO Try using memcpy or simple copying or raw byte data, if possible, to avoid handlling individual types.
            // Use type offset and size info.

            const size_t sizeOfType = objTypeInfo->size;

            // #TODO Consider creating an empty item here, instead of in each enum case.
            // That would mean special logic like setting the cJSON type would be needed below,
            // or more switch/if logic here (later probably better).
            cJSON* cJsonItem = nullptr;

            switch (objTypeInfo->enumType)
            {
            case MirrorTypes::m_string:
                {
                    const std::string* fieldAddress = (std::string*)obj;
                    cJsonItem = TestCreateString<const char>(name, fieldAddress->data()); // #TODO Requires ->data() so can't work with TestCreateString()
                }
                break;

            case MirrorTypes::eKeys:
            case MirrorTypes::m_char:
                {
                    char charArr[2] = { '\0', '\0'};
                    charArr[0] = *(char*)obj;
                    cJsonItem = cJSON_CreateString(charArr);
                    cJsonItem->string = _strdup(name.c_str());
                }
                break;
            case MirrorTypes::m_charPtr:
            case MirrorTypes::m_constCharPtr:
                cJsonItem = TestCreateString<const char>(name, *(void**)obj); break;
            case MirrorTypes::m_bool:
                cJsonItem = TestCreateBool<bool>(name, obj); break;
            case MirrorTypes::EditorWindowFlags:
            case MirrorTypes::EditorWindowTypes:
            case MirrorTypes::eScriptTypes: // #TODO Add a case for all enums by default
            case MirrorTypes::m_eSceneTypes:
            case MirrorTypes::m_uint8_t:
                cJsonItem = TestCreateNumber<uint8_t>(name, obj); break;
            case MirrorTypes::m_uint16_t:
                cJsonItem = TestCreateNumber<uint16_t>(name, obj); break;
            case MirrorTypes::m_uint32_t:
                cJsonItem = TestCreateNumber<uint32_t>(name, obj); break;
            case MirrorTypes::m_int8_t:
                cJsonItem = TestCreateNumber<int8_t>(name, obj); break;
            case MirrorTypes::m_int16_t:
                cJsonItem = TestCreateNumber<int16_t>(name, obj); break;
            case MirrorTypes::m_int:
            case MirrorTypes::m_int32_t:
                cJsonItem = TestCreateNumber<int32_t>(name, obj); break;
            case MirrorTypes::m_int64_t: // #NOTE Special case of conversion on 64 bit types
            {
                // Use string instead of a double to avoid conversion issues
                int64_t* numberAddress = (int64_t*)obj;
                cJsonItem = TestCreateString<const char>(name, std::to_string(*numberAddress).c_str());
                // #TODO Try using objJson->valuedouble and a memcpy to see if that works
            }
            // cJsonItem = TestCreateNumber<int64_t>(name, obj); break;
            break;

            case MirrorTypes::m_uint64_t: // #NOTE Special case of conversion on 64 bit types
                {
                    // Use string instead of a double to avoid conversion issues
                    uint64_t* numberAddress = (uint64_t*)obj;
                    cJsonItem = TestCreateString<const char>(name, std::to_string(*numberAddress).c_str());
                    // #TODO Try using objJson->valuedouble and a memcpy to see if that works
                }
                // cJsonItem = TestCreateNumber<uint64_t>(name, obj);
                break;

            case MirrorTypes::m_float: // #TODO write with decimals
                cJsonItem = TestCreateNumber<float>(name, obj); break;
            case MirrorTypes::m_double:
                cJsonItem = TestCreateNumber<double>(name, obj); break;

            default:
                LOG_ERROR("{0} Unsupported user defined field type {1} {2}({3}) for serialization!", __FUNCTION__, name.c_str(), objTypeInfo->stringName.c_str(), (int)objTypeInfo->enumType);
                break;
            }

            if (cJsonItem)
            {
                cJSON_AddItemToArray(objJson, cJsonItem);
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
                    local_SerializeClass(obj, objTypeInfo->superTypeInfo, objJson);
                }
                else
                {
                    LOG_ERROR("{0} Invalid superTypeInfo!", __FUNCTION__);
                    return;
                }
            }

            if (objTypeInfo->fields.empty())
            {
                LOG_WARN("{0} Class has no serialized members", __FUNCTION__);
            }

            for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
            {
                const Mirror::Field& field = objTypeInfo->fields[i];

                if (field.serializationFlags & Mirror::FieldSerializationFlags::_InspectorOnly)
                    continue;

                const void* fieldAddress = (char*)obj + field.offset;

                if (field.typeInfo->isPrimitive()) // #TODO Review handling primitive here (dependent on field name)
                {
                    local_SerializePrimitive(fieldAddress, field.typeInfo, objJson, field.name);
                }
                else
                {
                    // #TODO This also checks if primitive so maybe avoid redundancy
                    SerializeToJson(fieldAddress, field.typeInfo, objJson, field.name);
                }
            }
        }

        void local_SerializeCollection(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name)
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

            cJSON* collectionJsonContainer = TestCreateObject(name.c_str());
            cJSON_AddItemToArray(objJson, collectionJsonContainer);

            switch (objTypeInfo->enumType)
            {
            case MirrorTypes::m_map_guid_entt:
            case MirrorTypes::m_map_eScriptTypes_ScriptablePtr:
            case MirrorTypes::m_umap_guid_editorWindowPtr:
            case MirrorTypes::m_umap_string_int32:
            case MirrorTypes::m_vec_string:
            case MirrorTypes::m_vec_pair_guid_string:
            case MirrorTypes::m_vec_char:
            case MirrorTypes::m_imvec4_array: // imgui types
            case MirrorTypes::m_arr_float16:
            case MirrorTypes::m_arr_float10:
                TestAgnostic(obj, objTypeInfo, collectionJsonContainer); break;

            case MirrorTypes::m_pair_eScriptTypes_ScriptablePtr:
                TestSerializePair<std::pair<eScriptTypes, Scriptable*>>(obj, collectionJsonContainer); break;
            case MirrorTypes::m_pair_guid_string:
                using PairGuidString = std::pair<GUID, std::string>;
                TestSerializePair<PairGuidString>(obj, collectionJsonContainer); break;
            case MirrorTypes::m_pair_guid_editorWindowPtr:
                TestSerializePair<std::pair<GUID, Editor::EditorWindow*>>(obj, collectionJsonContainer); break;
            case MirrorTypes::m_pair_string_int32:
                TestSerializePair<std::pair<std::string, s32>>(obj, collectionJsonContainer); break;

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
                cJSON_AddItemToArray(entityComponentsJsonArray, componentJsonItem);

                Component& component = handle.GetComponent<Component>();
                local_SerializeClass(&component, componentTypeInfo, componentJsonItem);
            }(), ...);
        }

        template<typename... Component>
        static void NewSerializeComponents(TemplateArgumentList<Component...>, EntityHandle& handle, cJSON* entityComponentsJsonArray)
        {
            NewSerializeComponent<Component...>(handle, entityComponentsJsonArray);
        }

        void TestAgnostic(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson)
        {
            if (objTypeInfo->isPair())
            {
                int bp = 0;
            }

            size_t counter = 0;
            void* elementAddress = (void*)objTypeInfo->typeIterateCurrentFunc(obj, counter);
            while (elementAddress)
            {
                if (auto first = objTypeInfo->CollectionTypeInfoFirst())
                {
                    SerializeToJson(elementAddress, objTypeInfo->CollectionTypeInfoFirst(), objJson, objTypeInfo->CollectionTypeInfoFirst()->stringName);
                }
                else
                {
                    LOG_WARN("{0} First info is null!", __FUNCTION__);
                }
                ++counter;
                elementAddress = (void*)objTypeInfo->typeIterateCurrentFunc(obj, counter);
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
                std::string x; // #NOTE Handles keys of string and non-string types
                if (MirrorTypes::m_string == absoluteFirstTypeInfo->enumType)
                {
                    std::string* str = (std::string*)&objPair->first;
                    x = *str;
                }
                else
                {
                    u32* number = (u32*)&objPair->first;
                    x = std::to_string(*number);
                }
                local_SerializePrimitive(absoluteObjectAddress, absoluteSecondTypeInfo, collectionJsonContainer, x.c_str());
            }
            else if (absoluteSecondTypeInfo->isClass)
            {
                cJSON* classJsonItem = TestCreateObject(absoluteSecondTypeInfo->stringName.c_str());

                cJSON_AddItemToArray(collectionJsonContainer, classJsonItem);

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
                local_SerializeCollection(absoluteObjectAddress, absoluteSecondTypeInfo, collectionJsonItem, absoluteSecondTypeInfo->stringName);
            }
        }

        template <typename T>
        cJSON* TestCreateBool(const std::string& name, const void* obj) // #TODO Rename from "Test"
        {
            cJSON* returnBool = cJSON_CreateBool(*(T*)obj);
            returnBool->string = _strdup(name.c_str()); // #TODO Check memory de-allocation
            return returnBool;
        }

        template <typename T>
        cJSON* TestCreateNumber(const std::string& name, const void* obj) // #TODO Rename from "Test"
        {
            // Old
            // float* numberAddress = (float*)((char*)obj + field.offset);
            // AddItemToArray(objJson, CreateNumber(field.name.c_str(), *numberAddress));

            // CreateNumber(const char*, double)
            // cJSON* returnNumber = cJSON_CreateNumber(value);
            // returnNumber->string = _strdup(key);

            T test = *(T*)obj;
            double cache = test;
            cJSON* returnNumber = cJSON_CreateNumber(cache);
            returnNumber->string = _strdup(name.c_str());
            return returnNumber;
        }

        template <typename T>
        cJSON* TestCreateString(const std::string& name, const void* obj) // #TODO Rename from "Test"
        {
            cJSON* returnString = cJSON_CreateString((T*)obj); // #NOTE (T*)obj is not dereferenced
            returnString->string = _strdup(name.c_str());
            return returnString;
        }

    }

}
