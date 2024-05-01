#include "QF_Serialization.h"

#include <unordered_map>
#include <string>

#ifdef _QDEARIMGUI
#include "Libraries/imgui/imgui.h"
#endif

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MirrorTesting.h"
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
        void local_SerializePair(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson);

        cJSON* TestCreateObject(const char* key = nullptr);

        template<typename... Component> // #TODO Consider moving to QF_ComponentHelpers.h
        void NewSerializeComponent(EntityHandle& handle, cJSON* entityComponentsJsonArray);
        template<typename... Component>
        static void NewSerializeComponents(TemplateArgumentList<Component...>, EntityHandle& handle, cJSON* entityComponentsJsonArray);

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

                        NewSerializeComponents(EntityComponentsList{}, handle, entityComponentsJsonArray);

                        // SerializeComponent<ComponentCamera>(handle, entityComponentsJsonArray);
                        // SerializeComponent<ComponentInfo>(handle, entityComponentsJsonArray);
                        // SerializeComponent<ComponentMesh>(handle, entityComponentsJsonArray);
                        // SerializeComponent<ComponentTransform>(handle, entityComponentsJsonArray);
                        // SerializeComponent<ComponentScript>(handle, entityComponentsJsonArray);
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
                    cJSON* pointerJson = TestCreateObject(name.c_str());
                    cJSON_AddItemToArray(objJson, pointerJson);

                    if (nullptr == *(const void**)obj)
                    {
                        LOG_ERROR("{0} Pointer is null!", __FUNCTION__);
                        uint8_t pointerValue = 0;
                        local_SerializePrimitive(&pointerValue, Mirror::InfoForType<uint8_t>(), pointerJson, MIRROR_TO_STR(nullptr));
                        return;
                    }

                    const Mirror::TypeInfo* absoluteTypeInfo = objTypeInfo->AbsoluteType();
                    if (objTypeInfo->AbsoluteType()->hasSubClass())
                    {
                        if (objTypeInfo->AbsoluteType()->isAbstract) {}

                        for (const auto& pair : objTypeInfo->AbsoluteType()->derivedTypesMap)
                        {
                            if (pair.second->typeDynamicCastFunc(obj))
                            {
                                absoluteTypeInfo = pair.second;
                                break;
                            }
                        }
                    }

                    SerializeToJson(*(void**)obj, absoluteTypeInfo, pointerJson, absoluteTypeInfo->stringName);
                }
                break;

            case Mirror::TypeInfoCategories::TypeInfoCategory_Pair:
                local_SerializePair(obj, objTypeInfo, objJson); break;

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

            if (!objTypeInfo->isPair())
            {
                size_t counter = 0;
                void* elementAddress = (void*)objTypeInfo->typeIterateCurrentFunc(obj, counter);
                while (elementAddress)
                {
                    if (auto first = objTypeInfo->CollectionTypeInfoFirst())
                    {
                        SerializeToJson(elementAddress, objTypeInfo->CollectionTypeInfoFirst(), collectionJsonContainer, objTypeInfo->CollectionTypeInfoFirst()->stringName);
                    }
                    else
                    {
                        LOG_WARN("{0} First info is null!", __FUNCTION__);
                    }
                    ++counter;
                    elementAddress = (void*)objTypeInfo->typeIterateCurrentFunc(obj, counter);
                }
            }
            else
            {
                ASSERT(false, "Should not hit this line!");
            }
        }

        void local_SerializePair(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson)
        {
            cJSON* pairJson = TestCreateObject(objTypeInfo->stringName.c_str());
            cJSON_AddItemToArray(objJson, pairJson);

            const Mirror::TypeInfo* const objTypeInfoFirst = objTypeInfo->CollectionTypeInfoFirst();
            const Mirror::TypeInfo* const objTypeInfoSecond = objTypeInfo->CollectionTypeInfoSecond();

            const void* firstAddress = objTypeInfo->collectionFirstSecondFunc(obj, true);
            const void* secondAddress = objTypeInfo->collectionFirstSecondFunc(obj, false);

            if (objTypeInfoFirst->isPrimitive())
            {
                cJSON* pairFirstJson = TestCreateObject(objTypeInfoFirst->stringName.c_str());
                cJSON_AddItemToArray(pairJson, pairFirstJson);
                local_SerializePrimitive(firstAddress, objTypeInfoFirst, pairFirstJson, objTypeInfoFirst->stringName);
            }
            else
            {
                SerializeToJson(firstAddress, objTypeInfoFirst, pairJson, objTypeInfoFirst->stringName);
            }

            cJSON* pairSecondJson = TestCreateObject(objTypeInfoSecond->stringName.c_str());
            cJSON_AddItemToArray(pairJson, pairSecondJson);

            switch (objTypeInfoSecond->category)
            {
            case Mirror::TypeInfoCategories::TypeInfoCategory_Primitive:
                local_SerializePrimitive(secondAddress, objTypeInfoSecond, pairSecondJson, objTypeInfoSecond->stringName); break;

            case Mirror::TypeInfoCategories::TypeInfoCategory_Class:
                local_SerializeClass(secondAddress, objTypeInfoSecond, pairSecondJson); break;

            case Mirror::TypeInfoCategories::TypeInfoCategory_Collection:
                local_SerializeCollection(secondAddress, objTypeInfoSecond, pairSecondJson, objTypeInfoSecond->stringName); break;

            case Mirror::TypeInfoCategories::TypeInfoCategory_Pointer:
            {
                // #TODO Deduce derived type
                const Mirror::TypeInfo* secondAbsoluteTypeInfoDerived = objTypeInfoSecond->AbsoluteType();
                if (secondAbsoluteTypeInfoDerived->hasSubClass())
                {
                    for (const auto& pair : secondAbsoluteTypeInfoDerived->derivedTypesMap)
                    {
                        ASSERT(pair.second->typeDynamicCastFunc, "Null typeDynamicCastFunc!");
                        if (pair.second->typeDynamicCastFunc(secondAddress))
                        {
                            secondAbsoluteTypeInfoDerived = pair.second;
                            break;
                        }
                    }
                }
                SerializeToJson(*(void**)secondAddress, secondAbsoluteTypeInfoDerived, pairSecondJson, secondAbsoluteTypeInfoDerived->stringName);
            }
            break;

            case Mirror::TypeInfoCategories::TypeInfoCategory_Pair:
                local_SerializePair(secondAddress, objTypeInfoSecond, pairSecondJson); break;

            case Mirror::TypeInfoCategories::TypeInfoCategory_Invalid:
            default:
                break;
            }

            // if (objTypeInfoSecond->isPrimitive())
            // {
            //     local_SerializePrimitive(secondAddress, objTypeInfoSecond, pairSecondJson, objTypeInfoSecond->stringName);
            // }
            // else if (objTypeInfoSecond->isClass)
            // {
            //     local_SerializeClass(secondAddress, objTypeInfoSecond, pairSecondJson);
            // }
            // else if (objTypeInfoSecond->isCollection())
            // {
            //     local_SerializeCollection(secondAddress, objTypeInfoSecond, pairSecondJson, objTypeInfoSecond->stringName);
            // }
            // else if (objTypeInfoSecond->isPair())
            // {
            //     local_SerializePair(secondAddress, objTypeInfoSecond, pairSecondJson, objTypeInfoSecond->stringName);
            // }
            // else if (objTypeInfoSecond->isPointer)
            // {
            //     // #TODO Deduce derived type
            //     const Mirror::TypeInfo* secondAbsoluteTypeInfoDerived = objTypeInfoSecond->AbsoluteType();
            //     if (secondAbsoluteTypeInfoDerived->hasSubClass())
            //     {
            //         for (const auto& pair : secondAbsoluteTypeInfoDerived->derivedTypesMap)
            //         {
            //             ASSERT(pair.second->typeDynamicCastFunc, "Null typeDynamicCastFunc!");
            //             if (pair.second->typeDynamicCastFunc(secondAddress))
            //             {
            //                 secondAbsoluteTypeInfoDerived = pair.second;
            //                 break;
            //             }
            //         }
            //     }
            //     SerializeToJson(*(void**)secondAddress, secondAbsoluteTypeInfoDerived, pairSecondJson, secondAbsoluteTypeInfoDerived->stringName);
            // }
            // else
            // {
            //     LOG_ERROR("{0} Unsupported type info!", __FUNCTION__);
            // }
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
