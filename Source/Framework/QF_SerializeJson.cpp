#include "QF_Serialize.h"

#include <string>
#include <unordered_map>

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
#include "QF_Log.h"
#include "QF_Scene.h"
#include "QF_ScriptHelpers.h"

// Editor types
#include "../Source/Editor/QE_EditorWindowHelpers.h"
#include "../Source/Editor/QE_Settings.h"

namespace QwerkE {

    namespace Serialize {

        void local_SerializePrimitive(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name);
        void local_SerializeClass(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson);
        void local_SerializeCollection(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name);
        void local_SerializePair(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson);

        cJSON* CreateJsonObject(const char* key = nullptr);

        template <typename T>
        cJSON* CreateJsonBool(const std::string& name, const void* obj);
        template <typename T>
        cJSON* CreateJsonNumber(const std::string& name, const void* obj);
        template <typename T>
        cJSON* CreateJsonString(const std::string& name, const void* obj);

        template<typename... Component>
        void SerializeComponent(const entt::registry* const registry, entt::entity entityId, cJSON* componentListJsonArray);

        template<typename... Component>
        static void SerializeComponents(TemplateArgumentList<Component...>, const entt::registry* const registry, entt::entity entityId, cJSON* componentListJsonArray);

        bool TypeInfoHasSerializeOverride(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson);

        void ToJson(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (TypeInfoHasSerializeOverride(obj, objTypeInfo, objJson))
                return;

            switch (objTypeInfo->category)
            {
            case Mirror::TypeInfoCategories::TypeInfoCategory_Primitive:
                local_SerializePrimitive(obj, objTypeInfo, objJson, objTypeInfo->stringName); break;
            case Mirror::TypeInfoCategories::TypeInfoCategory_Class:
                {
                    cJSON* classJson = CreateJsonObject(name.c_str());
                    cJSON_AddItemToArray(objJson, classJson);
                    local_SerializeClass(obj, objTypeInfo, classJson);
                }
                break;
            case Mirror::TypeInfoCategories::TypeInfoCategory_Collection:
                local_SerializeCollection(obj, objTypeInfo, objJson, name); break;
            case Mirror::TypeInfoCategories::TypeInfoCategory_Pointer:
                {
                    cJSON* pointerJson = CreateJsonObject(name.c_str());
                    cJSON_AddItemToArray(objJson, pointerJson);

                    if (nullptr == *(const void**)obj)
                    {
                        LOG_ERROR("{0} Pointer is null!", __FUNCTION__);
                        uint8_t pointerValue = 0;
                        local_SerializePrimitive(&pointerValue, Mirror::InfoForType<uint8_t>(), pointerJson, MIRROR_TO_STR(nullptr));
                        return;
                    }

                    const Mirror::TypeInfo* absoluteTypeInfo = objTypeInfo->AbsoluteType()->DerivedTypeFromPointer(obj);
                    ToJson(*(void**)obj, absoluteTypeInfo, pointerJson, absoluteTypeInfo->stringName);
                }
                break;

            case Mirror::TypeInfoCategories::TypeInfoCategory_Pair:
                local_SerializePair(obj, objTypeInfo, objJson); break;
            }
        }

        void local_SerializePrimitive(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            cJSON* cJsonItem = nullptr;

            switch (objTypeInfo->enumType)
            {
            case MirrorTypes::m_string:
                {
                    const std::string* fieldAddress = (std::string*)obj;
                    cJsonItem = CreateJsonString<const char>(name, fieldAddress->data()); // #TODO Requires ->data() so can't work with CreateJsonString()
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
                cJsonItem = CreateJsonString<const char>(name, *(void**)obj); break;
            case MirrorTypes::m_bool:
                cJsonItem = CreateJsonBool<bool>(name, obj); break;
            case MirrorTypes::EditorWindowFlags:
            case MirrorTypes::EditorWindowTypes:
            case MirrorTypes::eScriptTypes: // #TODO Add a case for all enums by default
            case MirrorTypes::m_eSceneTypes:
            case MirrorTypes::m_uint8_t:
                cJsonItem = CreateJsonNumber<uint8_t>(name, obj); break;
            case MirrorTypes::m_uint16_t:
                cJsonItem = CreateJsonNumber<uint16_t>(name, obj); break;
            case MirrorTypes::m_uint32_t:
                cJsonItem = CreateJsonNumber<uint32_t>(name, obj); break;
            case MirrorTypes::m_int8_t:
                cJsonItem = CreateJsonNumber<int8_t>(name, obj); break;
            case MirrorTypes::m_int16_t:
                cJsonItem = CreateJsonNumber<int16_t>(name, obj); break;
            case MirrorTypes::m_int:
            case MirrorTypes::m_int32_t:
                cJsonItem = CreateJsonNumber<int32_t>(name, obj); break;
            case MirrorTypes::m_int64_t: // #NOTE Special case of conversion on 64 bit types
            {
                // Use string instead of a double to avoid conversion issues
                int64_t* numberAddress = (int64_t*)obj;
                cJsonItem = CreateJsonString<const char>(name, std::to_string(*numberAddress).c_str());
                // #TODO Try using objJson->valuedouble and a memcpy to see if that works
            }
            // cJsonItem = CreateJsonNumber<int64_t>(name, obj); break;
            break;

            case MirrorTypes::m_uint64_t: // #NOTE Special case of conversion on 64 bit types
                {
                    // Use string instead of a double to avoid conversion issues
                    uint64_t* numberAddress = (uint64_t*)obj;
                    cJsonItem = CreateJsonString<const char>(name, std::to_string(*numberAddress).c_str());
                    // #TODO Try using objJson->valuedouble and a memcpy to see if that works
                }
                // cJsonItem = CreateJsonNumber<uint64_t>(name, obj);
                break;

            case MirrorTypes::m_float: // #TODO write with decimals
                cJsonItem = CreateJsonNumber<float>(name, obj); break;
            case MirrorTypes::m_double:
                cJsonItem = CreateJsonNumber<double>(name, obj); break;

            default:
                LOG_ERROR("{0} Unsupported user defined field type {1} {2}({3}) for serialization!", __FUNCTION__, name.c_str(), objTypeInfo->stringName.c_str(), (int)objTypeInfo->enumType);
                break;
            }

            if (cJsonItem)
            {
                cJSON_AddItemToArray(objJson, cJsonItem);
            }
        }

        void local_SerializeClass(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson)
        {
            if (!obj || !objTypeInfo || !objJson || objTypeInfo->category != Mirror::TypeInfoCategory_Class)
            {
                LOG_ERROR("{0} Invalid argument passed!", __FUNCTION__);
                return;
            }

            if (objTypeInfo->superTypeInfo)
            {
                local_SerializeClass(obj, objTypeInfo->superTypeInfo, objJson);
            }

            for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
            {
                const Mirror::Field& field = objTypeInfo->fields[i];

                if (field.serializationFlags & Mirror::FieldSerializationFlags::_InspectorOnly)
                    continue;

                const void* fieldAddress = (char*)obj + field.offset;

                // #TODO No if check needed?
                if (Mirror::TypeInfoCategory_Primitive == field.typeInfo->category)
                {   // #TODO Review handling primitive here (dependent on field name)
                    local_SerializePrimitive(fieldAddress, field.typeInfo, objJson, field.name);
                }
                else
                {   // #TODO This also checks if primitive so maybe avoid redundancy
                    ToJson(fieldAddress, field.typeInfo, objJson, field.name);
                }
            }
        }

        void local_SerializeCollection(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson || Mirror::TypeInfoCategory_Collection != objTypeInfo->category)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (false) // (!objTypeInfo->newIsCollection()) // || !objTypeInfo->collectionTypeInfo)
            {
                LOG_ERROR("{0} Invalid type info!", __FUNCTION__);
                return;
            }

            cJSON* collectionJsonContainer = CreateJsonObject(name.c_str());
            cJSON_AddItemToArray(objJson, collectionJsonContainer);

            size_t counter = 0;
            void* elementAddress = (void*)objTypeInfo->typeIterateCurrentFunc(obj, counter);
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
                elementAddress = (void*)objTypeInfo->typeIterateCurrentFunc(obj, counter);
            }
        }

        void local_SerializePair(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson)
        {
            cJSON* pairJson = CreateJsonObject(objTypeInfo->stringName.c_str());
            cJSON_AddItemToArray(objJson, pairJson);

            const Mirror::TypeInfo* const objTypeInfoFirst = objTypeInfo->collectionTypeInfoFirst;
            const Mirror::TypeInfo* const objTypeInfoSecond = objTypeInfo->collectionTypeInfoSecond;

            const void* const firstAddress = objTypeInfo->collectionFirstSecondFunc(obj, true);
            const void* const secondAddress = objTypeInfo->collectionFirstSecondFunc(obj, false);

            if (objTypeInfo->collectionTypeInfoFirst->category == Mirror::TypeInfoCategory_Primitive)
            {
                cJSON* pairFirstJson = CreateJsonObject(objTypeInfoFirst->stringName.c_str());
                cJSON_AddItemToArray(pairJson, pairFirstJson);
                local_SerializePrimitive(firstAddress, objTypeInfoFirst, pairFirstJson, objTypeInfoFirst->stringName);
            }
            else
            {
                ToJson(firstAddress, objTypeInfoFirst, pairJson, objTypeInfoFirst->stringName);
            }

            cJSON* pairSecondJson = CreateJsonObject(objTypeInfoSecond->stringName.c_str());
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
                const Mirror::TypeInfo* secondAbsoluteTypeInfoDerived = objTypeInfoSecond->AbsoluteType()->DerivedTypeFromPointer(secondAddress);
                ToJson(*(void**)secondAddress, secondAbsoluteTypeInfoDerived, pairSecondJson, secondAbsoluteTypeInfoDerived->stringName);
            }
            break;

            case Mirror::TypeInfoCategories::TypeInfoCategory_Pair:
                local_SerializePair(secondAddress, objTypeInfoSecond, pairSecondJson); break;
            }
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
                local_SerializeClass(&component, componentTypeInfo, componentJsonItem);
            }(), ...);
        }

        bool TypeInfoHasSerializeOverride(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson)
        {
            switch (objTypeInfo->enumType)
            {
            case MirrorTypes::m_entt_registry:
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

                cJSON* componentJsonArray = CreateJsonObject(componentTypeInfo->stringName.c_str());
                cJSON_AddItemToArray(componentListJsonArray, componentJsonArray);

                local_SerializeClass(&component, componentTypeInfo, componentJsonArray);
            }(), ...);
        }

        template<typename... Component>
        static void SerializeComponents(TemplateArgumentList<Component...>, const entt::registry* const registry, entt::entity entityId, cJSON* componentListJsonArray)
        {
            SerializeComponent<Component...>(registry, entityId, componentListJsonArray);
        }

    }

}
