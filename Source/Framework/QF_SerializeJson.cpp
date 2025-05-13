#include "QF_Serialize.h"

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
#include "QF_Log.h"
#include "QF_Mirror.h"

// Editor types // #TODO Remove Editor code from Framework domain
#include "../Source/Editor/QE_Mirror.h"

namespace QwerkE {

    namespace Serialize {

        bool Local_TypeInfoHandleOverride(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name);

        void Local_SerializePrimitive(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name);
        void Local_SerializeClass(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name);
        void Local_SerializeCollection(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name);
        void Local_SerializePointer(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name);

        cJSON* CreateJsonObject(const char* key = nullptr);

        template<typename... Component>
        static void SerializeComponents(TemplateArgumentList<Component...>, const entt::registry* const registry, entt::entity entityId, cJSON* componentListJsonArray);

        template <typename T>
        cJSON* CreateJsonBool(const std::string& name, const void* obj);
        template <typename T>
        cJSON* CreateJsonNumber(const std::string& name, const void* obj);
        template <typename T>
        cJSON* CreateJsonString(const std::string& name, const void* obj);

        void ToJson(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name)
        {
            ASSERT(obj && objTypeInfo && objJson, "Null argument passed!");

            {
                cJSON* objectJson = objJson;
                if (false) // #TODO Look at creating json objects here, with flag for ignoring object creation for special cases handled in caller
                {
                    objectJson = CreateJsonObject(name.c_str());
                    cJSON_AddItemToArray(objJson, objectJson);
                }
            }

            if (Local_TypeInfoHandleOverride(obj, objTypeInfo, objJson, name))
                return;

            switch (objTypeInfo->category)
            {
            case Mirror::TypeInfoCategories::TypeInfoCategory_Primitive:
                Local_SerializePrimitive(obj, objTypeInfo, objJson, name); break;
            case Mirror::TypeInfoCategories::TypeInfoCategory_Class:
                Local_SerializeClass(obj, objTypeInfo, objJson, name); break;
            case Mirror::TypeInfoCategories::TypeInfoCategory_Collection:
                Local_SerializeCollection(obj, objTypeInfo, objJson, name); break;
            case Mirror::TypeInfoCategories::TypeInfoCategory_Pointer:
                Local_SerializePointer(obj, objTypeInfo, objJson, name); break;
            }
        }

        bool Local_TypeInfoHandleOverride(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name)
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
                        ASSERT(registry->has<ComponentInfo>(entityId), "Missing ComponentInfo!");
                        ComponentInfo& info = registry->get<ComponentInfo>(entityId);

                        cJSON* componentListJsonArray = CreateJsonObject(info.m_EntityName.c_str());
                        cJSON_AddItemToArray(entitiesJsonArray, componentListJsonArray);

                        SerializeComponents(EntityComponentsList{}, registry, entityId, componentListJsonArray);
                    });
                }
                break;

            // #NOTE Treat some types as primitives
            // case Mirror::TypeId<GUID>(): // #TODO Look at treating a GUID as a primitive type, and use owner member name before m_Guid class member name
            case Mirror::TypeId<std::string>():
            case Mirror::TypeId<const char*>():
                Local_SerializePrimitive(obj, objTypeInfo, objJson, name); break;
            default:
                return false;
            }
            return true;
        }

        void Local_SerializePrimitive(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name)
        {
            ASSERT(obj && objTypeInfo && objJson, "Null argument passed!"); // #NOTE Don't compare category in case of override

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
            case Mirror::TypeId<Editor::EditorWindowTypes>(): // #TODO Remove editor types from framework files
            case Mirror::TypeId<Editor::EditorWindowFlags>(): // #TODO Remove editor types from framework files
            case Mirror::TypeId<Editor::EditorWindowSizingFlags>(): // #TODO Remove editor types from framework files
            case Mirror::TypeId<const eScriptTypes>():
            case Mirror::TypeId<eScriptTypes>():
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
            ASSERT(obj && objTypeInfo && objJson && objTypeInfo->category == Mirror::TypeInfoCategory_Class, "Invalid argument passed!");

            cJSON* classJson = objJson;
            if (objTypeInfo->superTypeInfo) // #TODO Can look to move CreateJsonObject to ToJson
            {
                Local_SerializeClass(obj, objTypeInfo->superTypeInfo, objJson, name);
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

                if (field.flags & FieldSerializationFlags::_InspectorOnly) // #TODO Review editor logic
                    continue;

                const void* fieldAddress = (char*)obj + field.offset;
                ToJson(fieldAddress, field.typeInfo, classJson, field.name);
            }
        }

        void Local_SerializeCollection(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name)
        {
            ASSERT(obj && objTypeInfo && objJson && objTypeInfo->category == Mirror::TypeInfoCategory_Collection, "Invalid argument passed!");

            cJSON* collectionJsonContainer = CreateJsonObject(name.c_str());
            cJSON_AddItemToArray(objJson, collectionJsonContainer);

            cJSON* collectionItemJsonContainer = collectionJsonContainer;
            const Mirror::TypeInfo* currentTypeInfo = objTypeInfo->collectionTypeInfoFirst;
            size_t currentTypeInfoItemCount = 0;

            while (const void* elementAddress = (const void*)objTypeInfo->collectionIterateCurrentFunc(obj, currentTypeInfoItemCount++))
            {
                // #TODO Avoid special json formatting logic that corrects Local_TypeInfoHandleOverride()
                if (Mirror::TypeId<std::vector<std::string>>() != objTypeInfo->id && (
                    Mirror::TypeInfoCategories::TypeInfoCategory_Primitive == currentTypeInfo->category ||
                    Mirror::TypeId<std::string>() == currentTypeInfo->id ||
                    Mirror::TypeId<const char*>() == currentTypeInfo->id))
                {
                    collectionItemJsonContainer = CreateJsonObject(currentTypeInfo->stringName.c_str());
                    cJSON_AddItemToArray(collectionJsonContainer, collectionItemJsonContainer);
                }

                ToJson(elementAddress, currentTypeInfo, collectionItemJsonContainer, currentTypeInfo->stringName);

                if (currentTypeInfo == objTypeInfo->collectionTypeInfoFirst && objTypeInfo->collectionTypeInfoSecond) // Pair and tuple logic. Multiple types (1st, 2nd, etc) each of 1 count
                {
                    currentTypeInfo = objTypeInfo->collectionTypeInfoSecond; // #NOTE Increment type when handling pairs. Can evolve to ++typeInfo to support tuples as well
                }
            }
        }

        void Local_SerializePointer(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name)
        {
            ASSERT(obj && objTypeInfo && objJson && objTypeInfo->category == Mirror::TypeInfoCategory_Pointer, "Invalid argument passed!");

            cJSON* pointerJson = CreateJsonObject(name.c_str());
            cJSON_AddItemToArray(objJson, pointerJson);

            if (nullptr == *(const void**)obj) // #TODO Review null pointer value handling
            {
                LOG_ERROR("{0} Pointer is null!", __FUNCTION__);
                uint8_t pointerValue = 0;
                Local_SerializePrimitive(&pointerValue, Mirror::InfoForType<uint8_t>(), pointerJson, "nullptr");
                return;
            }

            const Mirror::TypeInfo* absoluteTypeInfo = objTypeInfo->AbsoluteType()->DerivedTypeFromPointer(obj);
            ToJson(*(void**)obj, absoluteTypeInfo, pointerJson, absoluteTypeInfo->stringName);
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

                    Local_SerializeClass(&component, componentTypeInfo, componentListJsonArray, componentTypeInfo->stringName);
                }(), ...);
        }

        template<typename... Component>
        static void SerializeComponents(TemplateArgumentList<Component...>, const entt::registry* const registry, entt::entity entityId, cJSON* componentListJsonArray)
        {
            SerializeComponent<Component...>(registry, entityId, componentListJsonArray);
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

    }

}
