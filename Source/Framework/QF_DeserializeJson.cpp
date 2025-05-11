#include "QF_Serialize.h"

#include <unordered_map>

#include "QF_Buffer.h"
#include "QF_ComponentHelpers.h"
#include "QF_EntityHandle.h"
#include "QF_Mirror.h"

// #if QWERKE_EDITOR // #TODO Review removing editor code from framework
#include "../Source/Editor/QE_EditorWindowHelpers.h"
#include "../Source/Editor/QE_Mirror.h"
// #endif // QWERKE_EDITOR

namespace QwerkE {

    namespace Serialize {

        void local_DeserializePrimitive(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj);
        void local_DeserializeClass(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj); // TODO Review adding const std::string& name);
        void local_DeserializeCollection(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, const std::string& name);
        void local_DeserializePointer(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, const std::string& name);

        bool local_TypeInfoHandleOverride(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj);

        template<typename... Component>
        static void DeserializeComponents(TemplateArgumentList<Component...>, entt::registry* registry, const cJSON* entityComponentsJsonArray);

        void FromJson(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (local_TypeInfoHandleOverride(objJson, objTypeInfo, obj))
                return;

            switch (objTypeInfo->category)
            {
            case Mirror::TypeInfoCategories::TypeInfoCategory_Primitive:
                local_DeserializePrimitive(objJson, objTypeInfo, obj); break;
            case Mirror::TypeInfoCategories::TypeInfoCategory_Class:
                local_DeserializeClass(objJson, objTypeInfo, obj); break;
            case Mirror::TypeInfoCategories::TypeInfoCategory_Collection:
                local_DeserializeCollection(objJson, objTypeInfo, obj, objTypeInfo->stringName); break;
            case Mirror::TypeInfoCategories::TypeInfoCategory_Pointer:
                local_DeserializePointer(objJson, objTypeInfo, obj, objTypeInfo->stringName); break;
            }
        }

        void local_DeserializePrimitive(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            std::string* str = (std::string*)obj;
            switch (objTypeInfo->id)
            {
            case Mirror::TypeId<const std::string>():
            case Mirror::TypeId<std::string>(): // #TODO Handles when obj is pair.first better. This will break if key and value are both string/char*
                {
                    objTypeInfo->typeConstructorFunc(obj); // #NOTE String must be constructed before assigned to
                    *(std::string*)obj = objJson->valuestring ? objJson->valuestring : objJson->string; break;
                }
            case Mirror::TypeId<const char*>():
            case Mirror::TypeId<char*>():
                *(const char**)obj = _strdup(objJson->valuestring); break;

            case Mirror::TypeId<const uint64_t>():
            case Mirror::TypeId<uint64_t>(): // #NOTE Storing uint64 as string
                *(uint64_t*)obj = std::stoull(objJson->valuestring); break;
            case Mirror::TypeId<const int64_t>():
            case Mirror::TypeId<int64_t>(): // #NOTE Storing int64 as string
                *(int64_t*)obj = std::stoll(objJson->valuestring); break;

            case Mirror::TypeId<const float>():
            case Mirror::TypeId<float>():
                *(float*)obj = (float)objJson->valuedouble; break; // #NOTE double to float loss of precision

            case Mirror::TypeId<QKey>():
            case Mirror::TypeId<const char>():
            case Mirror::TypeId<char>():
                *(char*)obj = (char)objJson->valuestring[0]; break; // #TODO Review char assignment

            default:
                if (objJson->type | cJSON_Number | cJSON_True | cJSON_False)
                {   // #TODO Potential bug writing to fieldAddress with size less than objJson->valueint
                    void* fieldAddress = ((char*)obj);
                    void* sourceAddress = (void*)&objJson->valueint;

                    if (objTypeInfo->size > 4)
                    {
                        sourceAddress = (void*)&objJson->valuedouble;
                    }
                    memcpy(fieldAddress, sourceAddress, objTypeInfo->size);
                }
                else
                {
                    LOG_WARN("{0} Unsupported json object type!", __FUNCTION__);
                }
                break;
            }
        }

        void local_DeserializeClass(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj)
        {
            if (!obj || !objTypeInfo || !objJson || Mirror::TypeInfoCategory_Class != objTypeInfo->category)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (objTypeInfo->superTypeInfo)
                local_DeserializeClass(objJson, objTypeInfo->superTypeInfo, obj);

            QwerkE::Editor::EditorWindow* window = (QwerkE::Editor::EditorWindow*)obj;

            if (objTypeInfo->typeConstructorFunc)
                objTypeInfo->typeConstructorFunc(obj);

            if (objTypeInfo->fields.empty())
                return;

            const cJSON* iterator = objJson->child;
            while (iterator)
            {
                for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
                {
                    const Mirror::Field& field = objTypeInfo->fields[i];

                    if (strcmp(field.name.c_str(), iterator->string) != 0
                        // #TODO Wrap editor only logic in #if Editor
#if QWERKE_EDITOR // #TODO Review removing editor code from framework
                        || field.flags & FieldSerializationFlags::_InspectorOnly)
#else
                        )
#endif // QWERKE_EDITOR
                        continue;

                    void* fieldAddress = (char*)obj + field.offset;

                    if (field.typeInfo->category == Mirror::TypeInfoCategory_Collection)
                    {   // #NOTE Needed to pass name of member collection
                        local_DeserializeCollection(iterator, field.typeInfo, fieldAddress, field.name);
                    }
                    else
                    {
                        FromJson(iterator, field.typeInfo, (char*)obj + field.offset);
                    }
                    break;
                }

                iterator = iterator->next;
            }
        }

        void local_DeserializeCollection(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (objTypeInfo->collectionTypeInfoSecond) // #TODO Refactor out pair specific logic
            {
                const Mirror::TypeInfo* const elementFirstTypeInfo = objTypeInfo->collectionTypeInfoFirst;
                Buffer elementFirstBuffer(elementFirstTypeInfo->size);
                const cJSON* firstJson = objJson->child;
                if (Mirror::TypeInfoCategory_Primitive == elementFirstTypeInfo->category)
                {   // #TODO Review handling primitives differently
                    firstJson = firstJson->child; // #NOTE Won't be wrapped in another cJSON object (like a class, collection, etc would)
                }
                FromJson(firstJson, elementFirstTypeInfo, elementFirstBuffer.As<void>());

                const Mirror::TypeInfo* elementSecondInfo = objTypeInfo->collectionTypeInfoSecond;
                Buffer elementSecondBuffer(elementSecondInfo ? elementSecondInfo->size : 0);
                const cJSON* secondJson = objJson->child->next;
                if (Mirror::TypeInfoCategory_Primitive == elementSecondInfo->category || // #TODO Fix std::string override
                    // #TODO Test const char*
                    Mirror::TypeId<std::string>() == elementSecondInfo->id ||
                    Mirror::TypeId<const char*>() == elementSecondInfo->id)
                {   // #TODO Review handling primitives differently
                    secondJson = secondJson->child; // #NOTE Won't be wrapped in another cJSON object (like a class, collection, etc would)
                }
                FromJson(secondJson, elementSecondInfo, elementSecondBuffer.As<void>());

                objTypeInfo->Construct(obj);
                objTypeInfo->CollectionAppend(obj, 0, elementFirstBuffer.As<void>(), elementSecondBuffer.As<void>());
                return;
            }

            if (false) // #TODO Implement
            {
                const Mirror::TypeInfo* currentTypeInfo = objTypeInfo->collectionTypeInfoFirst;
                while (currentTypeInfo)
                {
                    // #TOOD Improve logic for handling both first and last types without repeating code
                    Buffer elementFirstBuffer(currentTypeInfo->size);

                    currentTypeInfo->Construct(obj);
                    if (currentTypeInfo->collectionReserveFunc && currentTypeInfo->collectionCountFunc)
                    {
                        currentTypeInfo->collectionReserveFunc(obj, currentTypeInfo->collectionCountFunc(obj));
                    }

                    const cJSON* jsonIterator = objJson->child; // #TODO Review json pointer
                    size_t index = 0; // #NOTE Used for contiguous collections

                    while (jsonIterator)
                    {
                        FromJson(jsonIterator, currentTypeInfo, elementFirstBuffer.As<void>());
                        objTypeInfo->CollectionAppend(obj, index, elementFirstBuffer.As<void>(), nullptr);
                        jsonIterator = jsonIterator->next;
                        ++index;
                    }

                    if (currentTypeInfo->collectionTypeInfoSecond == currentTypeInfo)
                    {
                        break;
                    }
                    currentTypeInfo = currentTypeInfo->collectionTypeInfoSecond;
                }
            }

            const Mirror::TypeInfo* const elementFirstTypeInfo = objTypeInfo->collectionTypeInfoFirst;
            Buffer elementFirstBuffer(elementFirstTypeInfo->size);

            objTypeInfo->Construct(obj);
            if (objTypeInfo->collectionReserveFunc && objTypeInfo->collectionCountFunc)
            {
                objTypeInfo->collectionReserveFunc(obj, objTypeInfo->collectionCountFunc(obj));
            }

            // #DEBUG Registry
            auto vecLower = (std::pair<const size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>*)elementFirstBuffer.As<void>();
            std::unordered_map<size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>* map = (std::unordered_map<size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>*)obj;
            std::vector<QwerkE::Editor::EditorWindow*>* editorWindows = (std::vector<QwerkE::Editor::EditorWindow*>*)obj;

            const cJSON* jsonIterator = objJson->child;
            size_t index = 0; // #NOTE Used for contiguous collections

            while (jsonIterator)
            {
                // #DEBUG Registry
                std::vector<std::pair<QwerkE::GUID, std::string>>* vec = (std::vector<std::pair<QwerkE::GUID, std::string>>*)obj;
                std::pair<QwerkE::GUID, std::string>* pair = (std::pair<QwerkE::GUID, std::string>*)elementFirstBuffer.As<void>();
                QwerkE::Editor::EditorWindow* window = (QwerkE::Editor::EditorWindow*)elementFirstBuffer.As<void>();

                FromJson(jsonIterator, elementFirstTypeInfo, elementFirstBuffer.As<void>());
                objTypeInfo->CollectionAppend(obj, index, elementFirstBuffer.As<void>(), nullptr);
                jsonIterator = jsonIterator->next;
                ++index;
            }
        }

        void local_DeserializePointer(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (strcmp(objJson->child->string, "nullptr") == 0)
            {
                LOG_WARN("{0} Null pointer!", __FUNCTION__);
                *(void**)obj = nullptr;
                return;
            }

            const Mirror::TypeInfo* dereferencedTypeInfo = objTypeInfo->pointerDereferencedTypeInfo->DerivedTypeFromTypeName(objJson->child->string);
            void* derefencedTypeObjAddress = new char[dereferencedTypeInfo->size];
            *(void**)obj = derefencedTypeObjAddress;
            FromJson(objJson->child, dereferencedTypeInfo, derefencedTypeObjAddress);
            // if (constexpr bool pointersHaveObject = true)
            // {
            //     FromJson(objJson->child, dereferencedTypeInfo, derefencedTypeObjAddress);
            // }
            // else
            // {
            //     FromJson(objJson, dereferencedTypeInfo, derefencedTypeObjAddress);
            // }
        }

        bool local_TypeInfoHandleOverride(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj)
        {
            switch (objTypeInfo->id)
            {
            case Mirror::TypeId<entt::registry>():
                {
                    entt::registry* registry = (entt::registry*)obj;

                    const Mirror::TypeInfo* componentInfoTypeInfo = Mirror::InfoForType<ComponentInfo>();
                    const cJSON* iterator = objJson->child;
                    while (iterator)
                    {
                        DeserializeComponents(EntityComponentsList{}, registry, iterator);
                        iterator = iterator->next;
                    }
                }
                return true;

            // #NOTE Treat some types as primitives
            case Mirror::TypeId<std::string>():
            case Mirror::TypeId<const char*>():
                local_DeserializePrimitive(objJson, objTypeInfo, obj);
                return true;
            }
            return false;
        }

        template<typename... Component>
        void DeserializeComponent(entt::registry* registry, const cJSON* entityComponentsJsonArray)
        {
            entt::entity newEntityId = registry->create();
            ([&]()
            {
                const cJSON* iterator = entityComponentsJsonArray->child;
                while (iterator)
                {
                    const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<Component>();
                    if (strcmp(iterator->string, typeInfo->stringName.c_str()) != 0)
                    {
                        iterator = iterator->next;
                        continue;
                    }

                    Component& component = registry->emplace<Component>(newEntityId);

                    local_DeserializeClass(iterator, typeInfo, &component);
                    iterator = iterator->next;

                    if (std::is_same_v<Component, ComponentScript>)
                    {
                        // #TODO Ensure Scriptable* is null to trigger instantiation later
                        // #TODO Review nullifying pointers during deserialization
                        ComponentScript* scriptPtr = (ComponentScript*)&component;
                        for (auto& pair : scriptPtr->ScriptInstances())
                        {
                            pair.second = nullptr;
                        }
                    }

                    if (std::is_same_v<Component, ComponentMesh>)
                    {
                        ComponentMesh* mesh = (ComponentMesh*)&component;
                        mesh->Initialize();
                    }
                }
            }(), ...);
        }

        template<typename... Component>
        static void DeserializeComponents(TemplateArgumentList<Component...>, entt::registry* registry, const cJSON* entityComponentsJsonArray)
        {
            DeserializeComponent<Component...>(registry, entityComponentsJsonArray);
        }

    }

}
