#include "QF_Serialize.h"

#include <string>
#include <unordered_map>

#ifdef _QDEARIMGUI
#include "Libraries/imgui/imgui.h"
#endif

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

//#include "QC_Guid.h"

#include "QF_ComponentHelpers.h"
#include "QF_EntityHandle.h"
#include "QF_Log.h"
#include "QF_Mirror.h"
#include "QF_QKey.h"

// #if Framework
// #include "QF_Mirror.h"

// #if QWERKE_EDITOR // #TODO Review removing editor code from framework
#include "../Source/Editor/QE_Mirror.h"
// #endif // QWERKE_EDITOR

namespace QwerkE {

    namespace Serialize {

        void Local_SerializePrimitive(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name);
        void Local_SerializeClass(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name);
        void Local_SerializeCollection(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name);
        void Local_SerializePointer(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name);

        cJSON* CreateJsonObject(const char* key = nullptr);

        template <typename T>
        cJSON* CreateJsonBool(const std::string& name, const void* obj);
        template <typename T>
        cJSON* CreateJsonNumber(const std::string& name, const void* obj);
        template <typename T>
        cJSON* CreateJsonString(const std::string& name, const void* obj);

        template<typename... Component>
        static void SerializeComponents(TemplateArgumentList<Component...>, const entt::registry* const registry, entt::entity entityId, cJSON* componentListJsonArray);

        bool local_TypeInfoHandleOverride(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name);

        void ToJson(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (local_TypeInfoHandleOverride(obj, objTypeInfo, objJson, name))
                return;

            switch (objTypeInfo->category)
            {
            case Mirror::TypeInfoCategories::TypeInfoCategory_Primitive:
                Local_SerializePrimitive(obj, objTypeInfo, objJson, objTypeInfo->stringName); break;

            case Mirror::TypeInfoCategories::TypeInfoCategory_Class:
                Local_SerializeClass(obj, objTypeInfo, objJson, name); break;

            case Mirror::TypeInfoCategories::TypeInfoCategory_Collection:
                Local_SerializeCollection(obj, objTypeInfo, objJson, name); break;

            case Mirror::TypeInfoCategories::TypeInfoCategory_Pointer:
                Local_SerializePointer(obj, objTypeInfo, objJson, name); break;
            }
        }

        void Local_SerializePrimitive(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson) // #NOTE Don't check objTypeInfo->category in case of override
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            cJSON* cJsonItem = nullptr;

            switch (objTypeInfo->id)
            {
                // #TODO std::string is not a true primitive. Need a better way to handle this logic
            case Mirror::TypeId<const std::string>():
            case Mirror::TypeId<std::string>():
                {
                    const std::string* fieldAddress = (std::string*)obj;
                    cJsonItem = CreateJsonString<const char>(name, fieldAddress->data()); // #TODO Requires ->data() so can't work with CreateJsonString()
                }
                break;

            case Mirror::TypeId<QKey>():
            case Mirror::TypeId<const char>():
            case Mirror::TypeId<char>():
                {
                    char charArr[2] = { '\0', '\0'};
                    charArr[0] = *(char*)obj;
                    cJsonItem = cJSON_CreateString(charArr);
                    cJsonItem->string = _strdup(name.c_str());
                }
                break;
            case Mirror::TypeId<const char*>():
            case Mirror::TypeId<char*>():
                cJsonItem = CreateJsonString<const char>(name, *(void**)obj); break;
            case Mirror::TypeId<const bool>():
            case Mirror::TypeId<bool>():
                cJsonItem = CreateJsonBool<bool>(name, obj); break;
// #if QWERKE_EDITOR
            case Mirror::TypeId<Editor::EditorWindowTypes>(): // #TODO Remove editor types from framework files
            case Mirror::TypeId<Editor::EditorWindowFlags>(): // #TODO Remove editor types from framework files
            case Mirror::TypeId<Editor::EditorWindowSizingFlags>(): // #TODO Remove editor types from framework files
            case Mirror::TypeId<const eScriptTypes>():
            case Mirror::TypeId<eScriptTypes>():
// #endif // QWERKE_EDITOR
            case Mirror::TypeId<const uint8_t>():
            case Mirror::TypeId<uint8_t>():
                cJsonItem = CreateJsonNumber<uint8_t>(name, obj); break;
            case Mirror::TypeId<const uint16_t>():
            case Mirror::TypeId<uint16_t>():
                cJsonItem = CreateJsonNumber<uint16_t>(name, obj); break;
            case Mirror::TypeId<const uint32_t>():
            case Mirror::TypeId<uint32_t>():
                cJsonItem = CreateJsonNumber<uint32_t>(name, obj); break;
            case Mirror::TypeId<const int8_t>():
            case Mirror::TypeId<int8_t>():
                cJsonItem = CreateJsonNumber<int8_t>(name, obj); break;
            case Mirror::TypeId<const int16_t>():
            case Mirror::TypeId<int16_t>():
                cJsonItem = CreateJsonNumber<int16_t>(name, obj); break;
            case Mirror::TypeId<const int32_t>():
            case Mirror::TypeId<int32_t>():
                cJsonItem = CreateJsonNumber<int32_t>(name, obj); break;
            case Mirror::TypeId<const int64_t>():

            // case Mirror::TypeId<GUID>(): Handle GUID as primitive
            case Mirror::TypeId<int64_t>(): // #NOTE Special case of conversion on 64 bit types
            {
                int64_t* numberAddress = (int64_t*)obj;
                cJsonItem = CreateJsonString<const char>(name, std::to_string(*numberAddress).c_str());
                // #TODO Try using objJson->valuedouble and a memcpy to see if that works
            }
            break;

            case Mirror::TypeId<const uint64_t>():
            case Mirror::TypeId<uint64_t>(): // #NOTE Special case of conversion on 64 bit types
                {
                    uint64_t* numberAddress = (uint64_t*)obj;
                    cJsonItem = CreateJsonString<const char>(name, std::to_string(*numberAddress).c_str());
                    // #TODO Try using objJson->valuedouble and a memcpy to see if that works
                }
                break;

            case Mirror::TypeId<const float>():
            case Mirror::TypeId<float>(): // #TODO write with decimals
                cJsonItem = CreateJsonNumber<float>(name, obj); break;
            case Mirror::TypeId<const double>():
            case Mirror::TypeId<double>():
                cJsonItem = CreateJsonNumber<double>(name, obj); break;

            default:
                LOG_ERROR("{0} Unsupported user defined field type {1} {2}({3}) for serialization!", __FUNCTION__, name.c_str(), objTypeInfo->stringName.c_str(), (int)objTypeInfo->id);
                break;
            }

            if (cJsonItem)
            {
                cJSON_AddItemToArray(objJson, cJsonItem);
            }
        }

        void Local_SerializeClass(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson || Mirror::TypeInfoCategory_Class != objTypeInfo->category)
            {
                LOG_ERROR("{0} Invalid argument passed!", __FUNCTION__);
                return;
            }

            cJSON* classJson = objJson;
            if (objTypeInfo->superTypeInfo)
            {
                Local_SerializeClass(obj, objTypeInfo->superTypeInfo, classJson, name);
                classJson = classJson->child;
            }
            else
            {
                classJson = CreateJsonObject(name.c_str());
                cJSON_AddItemToArray(objJson, classJson);
            }

            for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
            {
                const Mirror::Field& field = objTypeInfo->fields[i];

// #if QWERKE_EDITOR
                if (field.flags & FieldSerializationFlags::_InspectorOnly)
                    continue;
// #endif // QWERKE_EDITOR

                const void* fieldAddress = (char*)obj + field.offset;

                // #TODO Ensure local_TypeInfoHasOverride() handles override logic
                // #TODO No if check needed?
                if (Mirror::TypeInfoCategory_Primitive == field.typeInfo->category)
                {   // #TODO Review handling primitive here (dependent on field name)
                    Local_SerializePrimitive(fieldAddress, field.typeInfo, classJson, field.name);
                }
                else
                {   // #TODO This also checks if primitive so maybe avoid redundancy
                    ToJson(fieldAddress, field.typeInfo, classJson, field.name);
                }
            }
        }

        void Local_SerializeCollection(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson || Mirror::TypeInfoCategory_Collection != objTypeInfo->category)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (objTypeInfo->collectionTypeInfoSecond) // #TODO Refactor pair only logic
            {
                cJSON* pairJson = CreateJsonObject(objTypeInfo->stringName.c_str());
                cJSON_AddItemToArray(objJson, pairJson);

                const Mirror::TypeInfo* const objTypeInfoFirst = objTypeInfo->collectionTypeInfoFirst;
                const Mirror::TypeInfo* const objTypeInfoSecond = objTypeInfo->collectionTypeInfoSecond;

                const void* const firstAddress = objTypeInfo->collectionAddressOfPairObjectFunc(obj, true);
                const void* const secondAddress = objTypeInfo->collectionAddressOfPairObjectFunc(obj, false);

                if (objTypeInfo->collectionTypeInfoFirst->category == Mirror::TypeInfoCategory_Primitive)
                {
                    cJSON* pairFirstJson = CreateJsonObject(objTypeInfoFirst->stringName.c_str());
                    cJSON_AddItemToArray(pairJson, pairFirstJson);
                    Local_SerializePrimitive(firstAddress, objTypeInfoFirst, pairFirstJson, objTypeInfoFirst->stringName);
                }
                else
                {
                    ToJson(firstAddress, objTypeInfoFirst, pairJson, objTypeInfoFirst->stringName);
                }

                cJSON* pairSecondJson = CreateJsonObject(objTypeInfoSecond->stringName.c_str());
                if (Mirror::TypeInfoCategories::TypeInfoCategory_Collection != objTypeInfoSecond->category)
                {
                    cJSON_AddItemToArray(pairJson, pairSecondJson);
                }

                // #TODO Fix std::string exception. Test const char*
                // #TODO Call ToJson to use ensure local_TypeInfoHasOverride() override logic
                // #TODO std::string has no fields so the value will never be written in local_SerializeClass()
                if (Mirror::TypeId<std::string>() == objTypeInfoSecond->id ||
                    Mirror::TypeId<const char*>() == objTypeInfoSecond->id)
                {
                    Local_SerializePrimitive(secondAddress, objTypeInfoSecond, pairSecondJson, objTypeInfoSecond->stringName);
                    return;
                }

                // #TODO Try to deprecate switch below with if/else above, or refactor switch
                switch (objTypeInfoSecond->category)
                {
                case Mirror::TypeInfoCategories::TypeInfoCategory_Primitive:
                    Local_SerializePrimitive(secondAddress, objTypeInfoSecond, pairSecondJson, objTypeInfoSecond->stringName); break;

                case Mirror::TypeInfoCategories::TypeInfoCategory_Class:
                    Local_SerializeClass(secondAddress, objTypeInfoSecond, pairSecondJson, objTypeInfoSecond->stringName); break;

                case Mirror::TypeInfoCategories::TypeInfoCategory_Collection:
                    // #NOTE Can't add because collection already adds
                    Local_SerializeCollection(secondAddress, objTypeInfoSecond, pairJson, objTypeInfoSecond->stringName); break;

                case Mirror::TypeInfoCategories::TypeInfoCategory_Pointer:
                    {
                        const Mirror::TypeInfo* secondAbsoluteTypeInfoDerived = objTypeInfoSecond->AbsoluteType()->DerivedTypeFromPointer(secondAddress);
                        ToJson(*(void**)secondAddress, secondAbsoluteTypeInfoDerived, pairSecondJson, secondAbsoluteTypeInfoDerived->stringName);
                    }
                    break;
                }
                return;
            }

            if (objTypeInfo->collectionTypeInfoSecond)
            {
                cJSON* pairJson = CreateJsonObject(objTypeInfo->stringName.c_str());
                cJSON_AddItemToArray(objJson, pairJson);

                const Mirror::TypeInfo* const objTypeInfoFirst = objTypeInfo->collectionTypeInfoFirst;
                const Mirror::TypeInfo* const objTypeInfoSecond = objTypeInfo->collectionTypeInfoSecond;

                const void* const firstAddress = objTypeInfo->collectionAddressOfPairObjectFunc(obj, true);
                const void* const secondAddress = objTypeInfo->collectionAddressOfPairObjectFunc(obj, false);

                if (objTypeInfo->collectionTypeInfoFirst->category == Mirror::TypeInfoCategory_Primitive)
                {
                    cJSON* pairFirstJson = CreateJsonObject(objTypeInfoFirst->stringName.c_str());
                    cJSON_AddItemToArray(pairJson, pairFirstJson);
                    Local_SerializePrimitive(firstAddress, objTypeInfoFirst, pairFirstJson, objTypeInfoFirst->stringName);
                }
                else
                {
                    ToJson(firstAddress, objTypeInfoFirst, pairJson, objTypeInfoFirst->stringName);
                }

                cJSON* pairSecondJson = CreateJsonObject(objTypeInfoSecond->stringName.c_str());
                if (Mirror::TypeInfoCategories::TypeInfoCategory_Collection != objTypeInfoSecond->category)
                {
                    cJSON_AddItemToArray(pairJson, pairSecondJson);
                }

                // #TODO Fix std::string exception. Test const char*
                // #TODO Call ToJson to use ensure local_TypeInfoHasOverride() override logic
                // #TODO std::string has no fields so the value will never be written in local_SerializeClass()
                if (Mirror::TypeId<std::string>() == objTypeInfoSecond->id ||
                    Mirror::TypeId<const char*>() == objTypeInfoSecond->id)
                {
                    Local_SerializePrimitive(secondAddress, objTypeInfoSecond, pairSecondJson, objTypeInfoSecond->stringName);
                    return;
                }

                // #TODO Try to deprecate switch below with if/else above, or refactor switch
                switch (objTypeInfoSecond->category)
                {
                case Mirror::TypeInfoCategories::TypeInfoCategory_Primitive:
                    Local_SerializePrimitive(secondAddress, objTypeInfoSecond, pairSecondJson, objTypeInfoSecond->stringName); break;

                case Mirror::TypeInfoCategories::TypeInfoCategory_Class:
                    Local_SerializeClass(secondAddress, objTypeInfoSecond, pairSecondJson, name); break;

                case Mirror::TypeInfoCategories::TypeInfoCategory_Collection:
                    // #NOTE Can't add because collection already adds
                    Local_SerializeCollection(secondAddress, objTypeInfoSecond, pairJson, objTypeInfoSecond->stringName); break;

                case Mirror::TypeInfoCategories::TypeInfoCategory_Pointer:
                {
                    const Mirror::TypeInfo* secondAbsoluteTypeInfoDerived = objTypeInfoSecond->AbsoluteType()->DerivedTypeFromPointer(secondAddress);
                    ToJson(*(void**)secondAddress, secondAbsoluteTypeInfoDerived, pairSecondJson, secondAbsoluteTypeInfoDerived->stringName);
                }
                break;
                }
                return;
            }

            cJSON* collectionJsonContainer = CreateJsonObject(name.c_str());
            cJSON_AddItemToArray(objJson, collectionJsonContainer);

            size_t counter = 0;
            void* elementAddress = (void*)objTypeInfo->collectionIterateCurrentFunc(obj, counter);
            while (elementAddress)
            {
                if (auto firstTypeInfo = objTypeInfo->collectionTypeInfoFirst)
                {
                    ToJson(elementAddress, firstTypeInfo, collectionJsonContainer, firstTypeInfo->stringName);
                }
                else
                {
                    LOG_WARN("{0} First info is null!", __FUNCTION__);
                }
                ++counter;
                elementAddress = (void*)objTypeInfo->collectionIterateCurrentFunc(obj, counter);
            }
        }

        void Local_SerializePointer(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson || Mirror::TypeInfoCategory_Pointer != objTypeInfo->category)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            // #TODO Consider avoiding a json object for pointers, and just use the dereferenced/absolute type instead?
            cJSON* pointerJson = objJson;
            if (constexpr bool pointersHaveObject = true)
            {
                pointerJson = CreateJsonObject(name.c_str());
                cJSON_AddItemToArray(objJson, pointerJson);
            }

            if (nullptr == *(const void**)obj)
            {
                LOG_ERROR("{0} Pointer is null!", __FUNCTION__);
                uint8_t pointerValue = 0;
                Local_SerializePrimitive(&pointerValue, Mirror::InfoForType<uint8_t>(), pointerJson, "nullptr");
                return;
            }

            const Mirror::TypeInfo* absoluteTypeInfo = objTypeInfo->AbsoluteType()->DerivedTypeFromPointer(obj);
            ToJson(*(void**)obj, absoluteTypeInfo, pointerJson, absoluteTypeInfo->stringName);
        }

        cJSON* CreateJsonObject(const char* key)
        {
            cJSON* returnJSONItem = cJSON_CreateObject();
            if (key) returnJSONItem->string = _strdup(key); // #TODO Look into deallocation responsibility
            return returnJSONItem;
        }

        template <typename T>
        cJSON* CreateJsonBool(const std::string& name, const void* obj)
        {
            cJSON* returnBool = cJSON_CreateBool(*(T*)obj);
            returnBool->string = _strdup(name.c_str()); // #TODO Check memory deallocation
            return returnBool;
        }

        template <typename T>
        cJSON* CreateJsonNumber(const std::string& name, const void* obj)
        {
            cJSON* returnNumber = cJSON_CreateNumber(*(T*)obj);
            returnNumber->string = _strdup(name.c_str());
            return returnNumber;
        }

        template <typename T>
        cJSON* CreateJsonString(const std::string& name, const void* obj)
        {
            cJSON* returnString = cJSON_CreateString((T*)obj); // #NOTE (T*)obj is not dereferenced
            returnString->string = _strdup(name.c_str());
            return returnString;
        }

        template<typename... Component> // #TODO Consider moving to QF_ComponentHelpers.h
        void SerializeComponent(EntityHandle& handle, cJSON* entityComponentsJsonArray)
        {
            ([&]()
            {
                if (!handle.HasComponent<Component>())
                    return;

                const Mirror::TypeInfo* componentTypeInfo = Mirror::InfoForType<Component>();

                cJSON* componentJsonItem = CreateJsonObject(componentTypeInfo->stringName.c_str());
                cJSON_AddItemToArray(entityComponentsJsonArray, componentJsonItem);

                Component& component = handle.GetComponent<Component>();
                Local_SerializeClass(&component, componentTypeInfo, componentJsonItem);
            }(), ...);
        }

        bool local_TypeInfoHandleOverride(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name)
        {
            switch (objTypeInfo->id)
            {
            case Mirror::TypeId<entt::registry>():
                {
                    cJSON* entitiesJsonArray = CreateJsonObject("m_Registry"); // #TODO Improve hard coded member name
                    cJSON_AddItemToArray(objJson, entitiesJsonArray);

                    entt::registry* registry = (entt::registry*)obj;
                    registry->each([&](auto entityId)
                    {
                        if (registry->has<ComponentInfo>(entityId))
                        {
                            ComponentInfo& info = registry->get<ComponentInfo>(entityId);

                            cJSON* componentListJsonArray = CreateJsonObject(info.m_EntityName.c_str());
                            cJSON_AddItemToArray(entitiesJsonArray, componentListJsonArray);

                            SerializeComponents(EntityComponentsList{}, registry, entityId, componentListJsonArray);
                        }
                        else
                        {
                            LOG_ERROR("{0} Missing ComponentInfo!", __FUNCTION__);
                        }
                    });
                }
                return true;

            // #NOTE Treat some types as primitives
            // case Mirror::TypeId<GUID>(): // #TODO Look at treating a GUID as a primitive type, and use owner member name before m_Guid class member name
            case Mirror::TypeId<std::string>():
            case Mirror::TypeId<const char*>():
                Local_SerializePrimitive(obj, objTypeInfo, objJson, name);
                return true;
            }
            return false;
        }

        template<typename... Component>
        void SerializeComponent(const entt::registry* const registry, entt::entity entityId, cJSON* componentListJsonArray)
        {
            ([&]()
            {
                if (!registry->has<Component>(entityId))
                    return;

                const Component& component = registry->get<Component>(entityId);
                const Mirror::TypeInfo* componentTypeInfo = Mirror::InfoForType<Component>();

                Local_SerializeClass(&component, componentTypeInfo, componentListJsonArray, componentTypeInfo->stringName.c_str());
            }(), ...);
        }

        template<typename... Component>
        static void SerializeComponents(TemplateArgumentList<Component...>, const entt::registry* const registry, entt::entity entityId, cJSON* componentListJsonArray)
        {
            SerializeComponent<Component...>(registry, entityId, componentListJsonArray);
        }

    }

}
