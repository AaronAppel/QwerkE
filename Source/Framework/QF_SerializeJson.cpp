#include "QF_Serialize.h"

#include <filesystem>
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

        bool Local_OverrideSerializeForType(const void* objectAddress, const Mirror::TypeInfo* objectTypeInfo, cJSON* objectJson, const std::string& typeOrMemberName);

        void Local_SerializePrimitive(const void* const primitiveAddress, const Mirror::TypeInfo* const primitiveTypeInfo, cJSON* primitiveJson, const std::string& primitiveTypeOrMemberName);
        void Local_SerializeClass(const void* const classAddress, const Mirror::TypeInfo* const classTypeInfo, cJSON* classJson);
        void Local_SerializeCollection(const void* const collectionAddress, const Mirror::TypeInfo* const collectionTypeInfo, cJSON* collectionJson);
        void Local_SerializePointer(const void* const pointerAddress, const Mirror::TypeInfo* const pointerTypeInfo, cJSON* pointerJson);

        template<typename... Component>
        static void SerializeComponents(TemplateArgumentList<Component...>, const entt::registry* const registry, entt::entity entityId, cJSON* componentListJsonArray);

        cJSON* CreateJsonObject(const char* typeOrMemberName = nullptr);
        template <typename T>
        cJSON* CreateJsonBool(const std::string& typeOrMemberName, const void* boolAddress);
        template <typename T>
        cJSON* CreateJsonNumber(const std::string& typeOrMemberName, const void* numberAddress);
        template <typename T>
        cJSON* CreateJsonString(const std::string& typeOrMemberName, const void* stringAddress);

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
                ASSERT(false, "Invalid or unhandled objectTypeInfo->category!");
            }
        }

        bool Local_OverrideSerializeForType(const void* objectAddress, const Mirror::TypeInfo* objectTypeInfo, cJSON* objectJson, const std::string& typeOrMemberName)
        {
            switch (objectTypeInfo->id)
            {
            case Mirror::IdForType<entt::registry>():
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
            // case Mirror::IdForType<GUID>(): // #TODO Look at treating a GUID as a primitive type, and use owner member name before m_Guid class member name
            case Mirror::IdForType<std::filesystem::path>():
            case Mirror::IdForType<std::string>():
            case Mirror::IdForType<const char*>():
                Local_SerializePrimitive(objectAddress, objectTypeInfo, objectJson, typeOrMemberName); break;
            default:
                return false;
            }
            return true;
        }

        void Local_SerializePrimitive(const void* const primitiveAddress, const Mirror::TypeInfo* const primitiveTypeInfo, cJSON* primitiveJson, const std::string& primitiveTypeOrMemberName)
        {
            ASSERT(primitiveAddress && primitiveTypeInfo && primitiveJson, "Null argument passed!"); // #NOTE Don't compare category in case of override

            cJSON* cJsonItem = nullptr;

            switch (primitiveTypeInfo->id)
            {
            case Mirror::IdForType<std::filesystem::path>(): // #TODO std::filesystem::path is not a true primitive. Need a better way to handle this logic
                {
                    const std::filesystem::path* fieldAddress = (std::filesystem::path*)primitiveAddress;
                    cJsonItem = CreateJsonString<const char>(primitiveTypeOrMemberName, fieldAddress->string().data());
                }
                break;
            case Mirror::IdForType<const std::string>(): // #TODO std::string is not a true primitive. Need a better way to handle this logic
            case Mirror::IdForType<std::string>():
                {
                    const std::string* fieldAddress = (std::string*)primitiveAddress;
                    cJsonItem = CreateJsonString<const char>(primitiveTypeOrMemberName, fieldAddress->data()); // #TODO Requires ->data() so can't work with CreateJsonString()
                }
                break;
            case Mirror::IdForType<QKey>():
            case Mirror::IdForType<const char>():
            case Mirror::IdForType<char>():
                {
                    char charArr[2] = { '\0', '\0'};
                    charArr[0] = *(char*)primitiveAddress;
                    cJsonItem = cJSON_CreateString(charArr);
                    cJsonItem->string = _strdup(primitiveTypeOrMemberName.c_str());
                }
                break;
            case Mirror::IdForType<const char*>():
            case Mirror::IdForType<char*>():
                cJsonItem = CreateJsonString<const char>(primitiveTypeOrMemberName, *(void**)primitiveAddress); break;
            case Mirror::IdForType<const bool>():
            case Mirror::IdForType<bool>():
                cJsonItem = CreateJsonBool<bool>(primitiveTypeOrMemberName, primitiveAddress); break;
            case Mirror::IdForType<Physics::BodyShapes>(): // #TODO Remove editor types from framework files
            case Mirror::IdForType<Editor::EditorWindowTypes>(): // #TODO Remove editor types from framework files
            case Mirror::IdForType<Editor::EditorWindowFlags>(): // #TODO Remove editor types from framework files
            case Mirror::IdForType<Editor::EditorWindowSizingFlags>(): // #TODO Remove editor types from framework files
            case Mirror::IdForType<const eScriptTypes>():
            case Mirror::IdForType<eScriptTypes>():
            case Mirror::IdForType<const uint8_t>():
            case Mirror::IdForType<uint8_t>():
                cJsonItem = CreateJsonNumber<uint8_t>(primitiveTypeOrMemberName, primitiveAddress); break;
            case Mirror::IdForType<const uint16_t>():
            case Mirror::IdForType<uint16_t>():
                cJsonItem = CreateJsonNumber<uint16_t>(primitiveTypeOrMemberName, primitiveAddress); break;
            case Mirror::IdForType<const uint32_t>():
            case Mirror::IdForType<uint32_t>():
                cJsonItem = CreateJsonNumber<uint32_t>(primitiveTypeOrMemberName, primitiveAddress); break;
            case Mirror::IdForType<const int8_t>():
            case Mirror::IdForType<int8_t>():
                cJsonItem = CreateJsonNumber<int8_t>(primitiveTypeOrMemberName, primitiveAddress); break;
            case Mirror::IdForType<const int16_t>():
            case Mirror::IdForType<int16_t>():
                cJsonItem = CreateJsonNumber<int16_t>(primitiveTypeOrMemberName, primitiveAddress); break;
            case Mirror::IdForType<const int32_t>():
            case Mirror::IdForType<int32_t>():
                cJsonItem = CreateJsonNumber<int32_t>(primitiveTypeOrMemberName, primitiveAddress); break;
            case Mirror::IdForType<const int64_t>():
            case Mirror::IdForType<int64_t>(): // #NOTE Special case of conversion on 64 bit types
            {
                int64_t* numberAddress = (int64_t*)primitiveAddress;
                cJsonItem = CreateJsonString<const char>(primitiveTypeOrMemberName, std::to_string(*numberAddress).c_str());
                // #TODO Try using objectJson->valuedouble and a memcpy to see if that works
            }
            break;

            case Mirror::IdForType<const uint64_t>():
            case Mirror::IdForType<uint64_t>(): // #NOTE Special case of conversion on 64 bit types
                {
                    uint64_t* numberAddress = (uint64_t*)primitiveAddress;
                    cJsonItem = CreateJsonString<const char>(primitiveTypeOrMemberName, std::to_string(*numberAddress).c_str());
                    // #TODO Try using objectJson->valuedouble and a memcpy to see if that works
                }
                break;

            case Mirror::IdForType<const float>():
            case Mirror::IdForType<float>(): // #TODO write with decimals
                cJsonItem = CreateJsonNumber<float>(primitiveTypeOrMemberName, primitiveAddress); break;
            case Mirror::IdForType<const double>():
            case Mirror::IdForType<double>():
                cJsonItem = CreateJsonNumber<double>(primitiveTypeOrMemberName, primitiveAddress); break;

            default:
                LOG_ERROR("{0} Unsupported user defined field type {1} {2}({3}) for serialization!", __FUNCTION__, primitiveTypeOrMemberName.c_str(), primitiveTypeInfo->stringName.c_str(), (int)primitiveTypeInfo->id);
                break;
            }

            if (cJsonItem)
            {
                cJSON_AddItemToArray(primitiveJson, cJsonItem);
            }
            else
            {
                LOG_ERROR("{0} Unsupported user defined field type {1} {2}({3}) for serialization!", __FUNCTION__, primitiveTypeOrMemberName.c_str(), primitiveTypeInfo->stringName.c_str(), (int)primitiveTypeInfo->id);
            }
        }

        void Local_SerializeClass(const void* const classAddress, const Mirror::TypeInfo* const classTypeInfo, cJSON* classJson)
        {
            ASSERT(classAddress && classTypeInfo && classJson && classTypeInfo->category == Mirror::TypeInfoCategory_Class, "Invalid argument passed!");

            if (classTypeInfo->superTypeInfo)
            {
                Local_SerializeClass(classAddress, classTypeInfo->superTypeInfo, classJson);
            }

            for (size_t i = 0; i < classTypeInfo->fields.size(); i++)
            {
                const Mirror::Field& field = classTypeInfo->fields[i];

                if (field.flags & FieldSerializationFlags::_InspectorOnly) // #TODO Review Editor logic in Framework domain
                    continue;

                const void* fieldAddress = (char*)classAddress + field.offset;
                ToJson(fieldAddress, field.typeInfo, classJson, field.name);
            }
        }

        void Local_SerializeCollection(const void* const collectionAddress, const Mirror::TypeInfo* const classTypeInfo, cJSON* classJson)
        {
            ASSERT(collectionAddress && classTypeInfo && classJson && classTypeInfo->category == Mirror::TypeInfoCategory_Collection, "Invalid argument passed!");

            const Mirror::TypeInfo* currentTypeInfo = classTypeInfo->collectionTypeInfoFirst;
            size_t currentTypeInfoItemCount = 0;

            while (const void* elementAddress = (const void*)classTypeInfo->collectionIterateCurrentFunc(collectionAddress, currentTypeInfoItemCount++))
            {
                ToJson(elementAddress, currentTypeInfo, classJson, currentTypeInfo->stringName);
                if (const bool isPair = currentTypeInfo == classTypeInfo->collectionTypeInfoFirst && classTypeInfo->collectionTypeInfoSecond)
                {
                    currentTypeInfo = classTypeInfo->collectionTypeInfoSecond; // #NOTE Increment type when handling pairs. Can evolve to ++typeInfo esque to support tuples as well
                }
            }
        }

        void Local_SerializePointer(const void* const pointerAddress, const Mirror::TypeInfo* const pointerTypeInfo, cJSON* pointerJson)
        {
            ASSERT(pointerAddress && pointerTypeInfo && pointerJson && pointerTypeInfo->category == Mirror::TypeInfoCategory_Pointer, "Invalid argument passed!");

            if (nullptr == *(const void**)pointerAddress) // #TODO Review null pointer value handling
            {
                LOG_ERROR("{0} Pointer is null!", __FUNCTION__);
                uint8_t pointerValue = 0;
                Local_SerializePrimitive(&pointerValue, Mirror::InfoForType<uint8_t>(), pointerJson, "nullptr");
                return;
            }

            const Mirror::TypeInfo* absoluteTypeInfo = pointerTypeInfo->AbsoluteType()->DerivedTypeFromPointer(pointerAddress);
            ToJson(*(void**)pointerAddress, absoluteTypeInfo, pointerJson, absoluteTypeInfo->stringName);
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

        cJSON* CreateJsonObject(const char* typeOrMemberName)
        {
            cJSON* returnJSONItem = cJSON_CreateObject();
            if (typeOrMemberName) returnJSONItem->string = _strdup(typeOrMemberName); // #TODO Look into deallocation responsibility
            return returnJSONItem;
        }

        template <typename T>
        cJSON* CreateJsonBool(const std::string& typeOrMemberName, const void* boolAddress)
        {
            cJSON* returnBool = cJSON_CreateBool(*(T*)boolAddress);
            returnBool->string = _strdup(typeOrMemberName.c_str()); // #TODO Check memory deallocation
            return returnBool;
        }

        template <typename T>
        cJSON* CreateJsonNumber(const std::string& typeOrMemberName, const void* numberAddress)
        {
            cJSON* returnNumber = cJSON_CreateNumber(*(T*)numberAddress);
            returnNumber->string = _strdup(typeOrMemberName.c_str());
            return returnNumber;
        }

        template <typename T>
        cJSON* CreateJsonString(const std::string& typeOrMemberName, const void* stringAddress)
        {
            cJSON* returnString = cJSON_CreateString((T*)stringAddress); // #NOTE (T*)obj is NOT dereferenced, unlike CreateJsonNumber() and CreateJsonBool()
            returnString->string = _strdup(typeOrMemberName.c_str());
            return returnString;
        }

    }

}
