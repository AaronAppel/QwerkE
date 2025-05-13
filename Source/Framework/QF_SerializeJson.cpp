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

// #if Framework
// #include "QF_Mirror.h"

// Editor types
// #TODO Review removing
// #if Editor
#include "../Source/Editor/QE_Mirror.h"

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

        bool Local_TypeInfoHandleOverride(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name);

        void ToJson(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (Local_TypeInfoHandleOverride(obj, objTypeInfo, objJson, name))
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
            if (!obj || !objTypeInfo || !objJson) // #NOTE Don't compare category in case of override
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
            if (!obj || !objTypeInfo || !objJson || objTypeInfo->category != Mirror::TypeInfoCategory_Class)
            {
                LOG_ERROR("{0} Invalid argument passed!", __FUNCTION__);
                return;
            }

            cJSON* classJson = objJson;
            if (objTypeInfo->superTypeInfo)
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

                if (field.flags & FieldSerializationFlags::_InspectorOnly)
                    continue;

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

            cJSON* collectionJsonContainer = CreateJsonObject(name.c_str());
            cJSON_AddItemToArray(objJson, collectionJsonContainer);

            // #TODO Finish new collection implementation
            const Mirror::TypeInfo* currentTypeInfo = objTypeInfo->collectionTypeInfoFirst;
            const u8 collectionTypesCount = objTypeInfo->collectionTypeInfoSecond ? 2 : 1; // #TODO Review logic
            for (size_t i = 0; i < collectionTypesCount; i++)
            {
                cJSON* collectionItemJsonContainer = collectionJsonContainer;

                // #TODO Improve special logic for json output formatting
                if (Mirror::TypeId<std::vector<std::string>>() != objTypeInfo->id && (
                    Mirror::TypeInfoCategories::TypeInfoCategory_Primitive == currentTypeInfo->category ||
                    Mirror::TypeId<std::string>() == currentTypeInfo->id ||
                    Mirror::TypeId<const char*>() == currentTypeInfo->id))
                {
                    collectionItemJsonContainer = CreateJsonObject(currentTypeInfo->stringName.c_str());
                    cJSON_AddItemToArray(collectionJsonContainer, collectionItemJsonContainer);
                }

                // objTypeInfo is a collection of a collection
                const bool isIterableCollection = currentTypeInfo->collectionIterateCurrentFunc;
                if (isIterableCollection)
                {
                    // #TODO objTypeInfo is a map and currentTypeInfo is a pair. Incorrect iterate logic
                    size_t counter = 0;
                    while (void* elementAddress = (void*)objTypeInfo->collectionIterateCurrentFunc(obj, counter++))
                    {
                        ToJson(elementAddress, currentTypeInfo, collectionItemJsonContainer, currentTypeInfo->stringName);
                    }
                }
                // objTypeInfo is vector, array, etc (1 dimensional)
                // #TODO Avoid caring about non-currentTypeInfo and handle each typeInfo separately
                else if (objTypeInfo->collectionIterateCurrentFunc && !objTypeInfo->collectionTypeInfoSecond)
                {
                    size_t currentTypeInfoItemCount = 0;
                    std::vector<std::string>* vec = (std::vector<std::string>*)obj;
                    while (void* elementAddress = (void*)objTypeInfo->collectionIterateCurrentFunc(obj, currentTypeInfoItemCount++))
                    {
                        std::string* string = (std::string*)elementAddress;
                        ToJson(elementAddress, objTypeInfo->collectionTypeInfoFirst, collectionItemJsonContainer, objTypeInfo->collectionTypeInfoFirst->stringName);
                    }
                }
                // objTypeInfo is just a pair/tuple?
                else
                {
                    const void* const objAddress = objTypeInfo->collectionIterateCurrentFunc(obj, i);
                    GUID* guid = (GUID*)objAddress;
                    std::string* string = (std::string*)objAddress;
                    ToJson(objAddress, currentTypeInfo, collectionItemJsonContainer, currentTypeInfo->stringName);
                }
                currentTypeInfo = objTypeInfo->collectionTypeInfoSecond;
            }
            return;
        }

        void Local_SerializePointer(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson || Mirror::TypeInfoCategory_Pointer != objTypeInfo->category)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            cJSON* pointerJson = CreateJsonObject(name.c_str());
            cJSON_AddItemToArray(objJson, pointerJson);

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

                Local_SerializeClass(&component, componentTypeInfo, componentListJsonArray, componentTypeInfo->stringName);
            }(), ...);
        }

        template<typename... Component>
        static void SerializeComponents(TemplateArgumentList<Component...>, const entt::registry* const registry, entt::entity entityId, cJSON* componentListJsonArray)
        {
            SerializeComponent<Component...>(registry, entityId, componentListJsonArray);
        }

    }

}
