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

// #TODO Remove Editor code from Framework domain
#include "../Source/Editor/QE_Mirror.h"

namespace QwerkE {

    namespace Serialize {

        bool Local_OverrideSerializeForType(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& typeOrMemberName);

        void Local_SerializePrimitive(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name);
        void Local_SerializeClass(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson);
        void Local_SerializeCollection(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson);
        void Local_SerializePointer(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson);

        template<typename... Component>
        static void SerializeComponents(TemplateArgumentList<Component...>, const entt::registry* const registry, entt::entity entityId, cJSON* componentListJsonArray);

        cJSON* CreateJsonObject(const char* key = nullptr);
        template <typename T>
        cJSON* CreateJsonBool(const std::string& name, const void* obj);
        template <typename T>
        cJSON* CreateJsonNumber(const std::string& name, const void* obj);
        template <typename T>
        cJSON* CreateJsonString(const std::string& name, const void* obj);

        void ToJson(const void* objectAddress, const Mirror::TypeInfo* objectTypeInfo, cJSON* objectJson, const std::string& typeOrMemberName)
        {
            ASSERT(objectAddress && objectTypeInfo && objectJson, "Null argument passed!");

            if (Local_OverrideSerializeForType(objectAddress, objectTypeInfo, objectJson, typeOrMemberName))
                return;

            cJSON* newObjectJson = objectJson;
            if (Mirror::TypeInfoCategories::TypeInfoCategory_Primitive != objectTypeInfo->category)
            {
                newObjectJson = CreateJsonObject(typeOrMemberName.c_str());
                cJSON_AddItemToArray(objectJson, newObjectJson);
            }

            switch (objectTypeInfo->category)
            {
            case Mirror::TypeInfoCategories::TypeInfoCategory_Primitive:
                Local_SerializePrimitive(objectAddress, objectTypeInfo, newObjectJson, typeOrMemberName); break;
            case Mirror::TypeInfoCategories::TypeInfoCategory_Class:
                Local_SerializeClass(objectAddress, objectTypeInfo, newObjectJson); break;
            case Mirror::TypeInfoCategories::TypeInfoCategory_Collection:
                Local_SerializeCollection(objectAddress, objectTypeInfo, newObjectJson); break;
            case Mirror::TypeInfoCategories::TypeInfoCategory_Pointer:
                Local_SerializePointer(objectAddress, objectTypeInfo, newObjectJson); break;
            default:
                ASSERT(false, "Invalid or unhandled objTypeInfo->category!");
            }
        }

        bool Local_OverrideSerializeForType(const void* objectAddress, const Mirror::TypeInfo* objectTypeInfo, cJSON* objectJson, const std::string& typeOrMemberName)
        {
            switch (objectTypeInfo->id)
            {
            case Mirror::TypeId<entt::registry>():
                {
                    cJSON* entitiesJsonArray = CreateJsonObject("m_Registry"); // #TODO Improve hard coded member name
                    cJSON_AddItemToArray(objectJson, entitiesJsonArray);

                    entt::registry* registry = (entt::registry*)objectAddress;
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
                Local_SerializePrimitive(objectAddress, objectTypeInfo, objectJson, typeOrMemberName); break;
            default:
                return false;
            }
            return true;
        }

        void Local_SerializePrimitive(const void* const objectAddress, const Mirror::TypeInfo* const objectTypeInfo, cJSON* objectJson, const std::string& typeOrMemberName)
        {
            ASSERT(objectAddress && objectTypeInfo && objectJson, "Null argument passed!"); // #NOTE Don't compare category in case of override

            cJSON* cJsonItem = nullptr;

            switch (objectTypeInfo->id)
            {
                // #TODO std::string is not a true primitive. Need a better way to handle this logic
            case Mirror::TypeId<const std::string>():
            case Mirror::TypeId<std::string>():
                {
                    const std::string* fieldAddress = (std::string*)objectAddress;
                    cJsonItem = CreateJsonString<const char>(typeOrMemberName, fieldAddress->data()); // #TODO Requires ->data() so can't work with CreateJsonString()
                }
                break;

            case Mirror::TypeId<QKey>():
            case Mirror::TypeId<const char>():
            case Mirror::TypeId<char>():
                {
                    char charArr[2] = { '\0', '\0'};
                    charArr[0] = *(char*)objectAddress;
                    cJsonItem = cJSON_CreateString(charArr);
                    cJsonItem->string = _strdup(typeOrMemberName.c_str());
                }
                break;
            case Mirror::TypeId<const char*>():
            case Mirror::TypeId<char*>():
                cJsonItem = CreateJsonString<const char>(typeOrMemberName, *(void**)objectAddress); break;
            case Mirror::TypeId<const bool>():
            case Mirror::TypeId<bool>():
                cJsonItem = CreateJsonBool<bool>(typeOrMemberName, objectAddress); break;
            case Mirror::TypeId<Editor::EditorWindowTypes>(): // #TODO Remove editor types from framework files
            case Mirror::TypeId<Editor::EditorWindowFlags>(): // #TODO Remove editor types from framework files
            case Mirror::TypeId<Editor::EditorWindowSizingFlags>(): // #TODO Remove editor types from framework files
            case Mirror::TypeId<const eScriptTypes>():
            case Mirror::TypeId<eScriptTypes>():
            case Mirror::TypeId<const uint8_t>():
            case Mirror::TypeId<uint8_t>():
                cJsonItem = CreateJsonNumber<uint8_t>(typeOrMemberName, objectAddress); break;
            case Mirror::TypeId<const uint16_t>():
            case Mirror::TypeId<uint16_t>():
                cJsonItem = CreateJsonNumber<uint16_t>(typeOrMemberName, objectAddress); break;
            case Mirror::TypeId<const uint32_t>():
            case Mirror::TypeId<uint32_t>():
                cJsonItem = CreateJsonNumber<uint32_t>(typeOrMemberName, objectAddress); break;
            case Mirror::TypeId<const int8_t>():
            case Mirror::TypeId<int8_t>():
                cJsonItem = CreateJsonNumber<int8_t>(typeOrMemberName, objectAddress); break;
            case Mirror::TypeId<const int16_t>():
            case Mirror::TypeId<int16_t>():
                cJsonItem = CreateJsonNumber<int16_t>(typeOrMemberName, objectAddress); break;
            case Mirror::TypeId<const int32_t>():
            case Mirror::TypeId<int32_t>():
                cJsonItem = CreateJsonNumber<int32_t>(typeOrMemberName, objectAddress); break;
            case Mirror::TypeId<const int64_t>():
            case Mirror::TypeId<int64_t>(): // #NOTE Special case of conversion on 64 bit types
            {
                int64_t* numberAddress = (int64_t*)objectAddress;
                cJsonItem = CreateJsonString<const char>(typeOrMemberName, std::to_string(*numberAddress).c_str());
                // #TODO Try using objJson->valuedouble and a memcpy to see if that works
            }
            break;

            case Mirror::TypeId<const uint64_t>():
            case Mirror::TypeId<uint64_t>(): // #NOTE Special case of conversion on 64 bit types
                {
                    uint64_t* numberAddress = (uint64_t*)objectAddress;
                    cJsonItem = CreateJsonString<const char>(typeOrMemberName, std::to_string(*numberAddress).c_str());
                    // #TODO Try using objJson->valuedouble and a memcpy to see if that works
                }
                break;

            case Mirror::TypeId<const float>():
            case Mirror::TypeId<float>(): // #TODO write with decimals
                cJsonItem = CreateJsonNumber<float>(typeOrMemberName, objectAddress); break;
            case Mirror::TypeId<const double>():
            case Mirror::TypeId<double>():
                cJsonItem = CreateJsonNumber<double>(typeOrMemberName, objectAddress); break;

            default:
                LOG_ERROR("{0} Unsupported user defined field type {1} {2}({3}) for serialization!", __FUNCTION__, typeOrMemberName.c_str(), objectTypeInfo->stringName.c_str(), (int)objectTypeInfo->id);
                break;
            }

            if (cJsonItem)
            {
                cJSON_AddItemToArray(objectJson, cJsonItem);
            }
            else
            {
                LOG_ERROR("{0} Unsupported user defined field type {1} {2}({3}) for serialization!", __FUNCTION__, typeOrMemberName.c_str(), objectTypeInfo->stringName.c_str(), (int)objectTypeInfo->id);
            }
        }

        void Local_SerializeClass(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson)
        {
            ASSERT(obj && objTypeInfo && objJson && objTypeInfo->category == Mirror::TypeInfoCategory_Class, "Invalid argument passed!");

            if (objTypeInfo->superTypeInfo)
            {
                Local_SerializeClass(obj, objTypeInfo->superTypeInfo, objJson);
            }

            for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
            {
                const Mirror::Field& field = objTypeInfo->fields[i];

                if (field.flags & FieldSerializationFlags::_InspectorOnly) // #TODO Review Editor logic in Framework domain
                    continue;

                const void* fieldAddress = (char*)obj + field.offset;
                ToJson(fieldAddress, field.typeInfo, objJson, field.name);
            }
        }

        void Local_SerializeCollection(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson)
        {
            ASSERT(obj && objTypeInfo && objJson && objTypeInfo->category == Mirror::TypeInfoCategory_Collection, "Invalid argument passed!");

            cJSON* collectionItemJsonContainer = objJson;
            const Mirror::TypeInfo* currentTypeInfo = objTypeInfo->collectionTypeInfoFirst;
            size_t currentTypeInfoItemCount = 0;

            while (const void* elementAddress = (const void*)objTypeInfo->collectionIterateCurrentFunc(obj, currentTypeInfoItemCount++))
            {
                ToJson(elementAddress, currentTypeInfo, collectionItemJsonContainer, currentTypeInfo->stringName);
                if (const bool isPair = currentTypeInfo == objTypeInfo->collectionTypeInfoFirst && objTypeInfo->collectionTypeInfoSecond)
                {
                    currentTypeInfo = objTypeInfo->collectionTypeInfoSecond; // #NOTE Increment type when handling pairs. Can evolve to ++typeInfo esque to support tuples as well
                }
            }
        }

        void Local_SerializePointer(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson)
        {
            ASSERT(obj && objTypeInfo && objJson && objTypeInfo->category == Mirror::TypeInfoCategory_Pointer, "Invalid argument passed!");

            if (nullptr == *(const void**)obj) // #TODO Review null pointer value handling
            {
                LOG_ERROR("{0} Pointer is null!", __FUNCTION__);
                uint8_t pointerValue = 0;
                Local_SerializePrimitive(&pointerValue, Mirror::InfoForType<uint8_t>(), objJson, "nullptr");
                return;
            }

            const Mirror::TypeInfo* absoluteTypeInfo = objTypeInfo->AbsoluteType()->DerivedTypeFromPointer(obj);
            ToJson(*(void**)obj, absoluteTypeInfo, objJson, absoluteTypeInfo->stringName);
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

                ToJson(&component, componentTypeInfo, componentListJsonArray, componentTypeInfo->stringName);
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
            cJSON* returnString = cJSON_CreateString((T*)obj); // #NOTE (T*)obj is NOT dereferenced, unlike CreateJsonNumber() and CreateJsonBool()
            returnString->string = _strdup(name.c_str());
            return returnString;
        }

    }

}
