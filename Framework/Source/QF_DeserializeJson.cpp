#include "QF_Serialize.h"

#include <unordered_map>

#include "QC_Guid.h"

#include "QF_Buffer.h"
#include "QF_ComponentHelpers.h"
#include "QF_EntityHandle.h"
#include "QF_Scene.h"

// Editor types
#include "../Editor/Source/QE_EditorWindowHelpers.h"

namespace QwerkE {

    namespace Serialize {

        void local_DeserializePrimitive(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, bool useNameString = false);
        void local_DeserializeClass(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj);
        void local_DeserializeCollection(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, const std::string& name);
        void local_DeserializePair(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, const std::string& name);

        template<typename... Component>
        void DeserializeComponent(EntityHandle& handle, const cJSON* entityComponentsJsonArray)
        {
            ([&]()
            {
                // #TODO Implement

                // bool addComponent = !std::is_same_v<T, ComponentTransform> && !std::is_same_v<T, ComponentInfo>;

                const Mirror::TypeInfo* componentTypeInfo = Mirror::InfoForType<Component>();
                // #TODO Iterate over entityComponentsJsonArray
                // if (strcmp(componentJson->string, componentTypeInfo->stringName.c_str()) != 0)
                //     return;
                //
                // if (addComponent)
                // {
                //     handle.AddComponent<T>();
                // }
                //
                // T& component = handle.GetComponent<T>();
                // DeserializeFromJson(componentJson, componentTypeInfo, (void*)&component);
                //
                // if (std::is_same_v<T, ComponentMesh>)
                // {
                //     ComponentMesh* mesh = (ComponentMesh*)&component;
                //     mesh->Initialize();
                // }
            }(), ...);
        }

        template<typename... Component>
        static void DeserializeComponents(TemplateArgumentList<Component...>, EntityHandle& handle, const cJSON* entityComponentsJsonArray)
        {
            DeserializeComponent<Component...>(handle, entityComponentsJsonArray);
        }

        s32 OffsetOfMember(const Mirror::TypeInfo* objTypeInfo, const char* memberName)
        {
            for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
            {
                const Mirror::Field& field = objTypeInfo->fields[i];
                if (strcmp(field.name.c_str(), memberName) == 0)
                {
                    return field.offset;
                }
            }
            return -1;
        }

        bool TypeInfoHasDeserializeOverride(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, const void* obj)
        {
            switch (objTypeInfo->enumType)
            {
            case MirrorTypes::m_map_guid_entt:
                {
                    // std::unordered_map<GUID, entt::entity>* entitiesMap = (std::unordered_map<GUID, entt::entity>*)obj;

                    const s32 offSetOfEntitiesMap = OffsetOfMember(Mirror::InfoForType<Scene>(), "m_GuidsToEntts");
                    if (offSetOfEntitiesMap < 0)
                    {
                        LOG_ERROR("{0} Offset not found!", __FUNCTION__);
                        return true;
                    }
                    Scene* scene = (Scene*)((char*)obj - offSetOfEntitiesMap);

                    const cJSON* iteratorEntities = objJson->child;
                    while (iteratorEntities)
                    {
                        GUID guid;
                        if (strcmp(iteratorEntities->string, "Entity") != 0)
                        {
                            guid = std::stoull(iteratorEntities->string);
                        }

                        EntityHandle handle = scene->CreateEntity(guid);

                        const cJSON* iteratorComponents = iteratorEntities->child;

                        DeserializeComponents(EntityComponentsList{}, handle, iteratorComponents);

                        while (iteratorComponents)
                        {   // #TODO Look at using component enum instead of strings
                            // DeserializeComponent<ComponentTransform>(handle, iteratorComponents, false); // #NOTE Added to every entity on creation
                            // DeserializeComponent<ComponentInfo>(handle, iteratorComponents, false);
                            // DeserializeComponent<ComponentCamera>(handle, iteratorComponents, true); // #NOTE Need to be added only if in data
                            // DeserializeComponent<ComponentMesh>(handle, iteratorComponents, true);
                            // DeserializeComponent<ComponentScript>(handle, iteratorComponents, true); // #TODO Consider using ComponentScript.AddScript()

                            iteratorComponents = iteratorComponents->next;
                        }

                        ASSERT(handle.HasComponent<ComponentTransform>(), "Entity must have ComponentTransform!");
                        ASSERT(handle.HasComponent<ComponentInfo>(), "Entity must have ComponentInfo!");

                        iteratorEntities = iteratorEntities->next;
                    }
                }
                return true;
            }
            return false;
        }

        void FromJson(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (TypeInfoHasDeserializeOverride(objJson, objTypeInfo, obj))
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
                {
                    if (strcmp(objJson->child->string, MIRROR_TO_STR(nullptr)) == 0)
                    {
                        LOG_WARN("{0} Null pointer!", __FUNCTION__);
                        *(void**)obj = nullptr;
                        return;
                    }

                    const Mirror::TypeInfo* dereferencedTypeInfo = objTypeInfo->pointerDereferencedTypeInfo->DerivedTypeFromTypeName(objJson->child->string);

                    void* derefencedTypeObjAddress = *(void**)obj;
                    derefencedTypeObjAddress = new char[dereferencedTypeInfo->size];
                    *(void**)obj = derefencedTypeObjAddress;
                    FromJson(objJson->child, dereferencedTypeInfo, derefencedTypeObjAddress);

                    // #TODO Fix edtor window guids being 0
                    Editor::EditorWindow* window = (Editor::EditorWindow*)derefencedTypeObjAddress;
                    GUID guid = window->Guid();
                    dereferencedTypeInfo->Construct(derefencedTypeObjAddress);
                    int bp = 0;
                }
                break;

            case Mirror::TypeInfoCategories::TypeInfoCategory_Pair:
                local_DeserializePair(objJson, objTypeInfo, obj, objTypeInfo->stringName); break;
            }
        }

        void local_DeserializePrimitive(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, bool isPairFirst)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            switch (objTypeInfo->enumType)
            {
            case MirrorTypes::m_string: // #TODO Handles when obj is pair.first better. This will break if key and value are both string/char*
                objTypeInfo->typeConstructorFunc(obj); // #NOTE String must be constructed before assigned
                *(std::string*)obj = objJson->valuestring ? objJson->valuestring : objJson->string; break;
            case MirrorTypes::m_charPtr:
            case MirrorTypes::m_constCharPtr:
                *(const char**)obj = _strdup(objJson->valuestring); break;
            case MirrorTypes::m_uint64_t: // #NOTE Storing uint64 as string
                *(uint64_t*)obj = std::stoull(objJson->valuestring); break;
            case MirrorTypes::m_int64_t: // #NOTE Storing int64 as string
                *(int64_t*)obj = std::stoll(objJson->valuestring); break;
            case MirrorTypes::m_float:
                *(float*)obj = objJson->valuedouble; break;

            default:
                if (objJson->type | cJSON_Number | cJSON_True | cJSON_False)
                {   // #TODO Potential bug writing to fieldAddress with size less than objJson->valueint
                    void* fieldAddress = ((char*)obj);
                    void* sourceAddress = (void*)&objJson->valueint;

                    if (objTypeInfo->size > 4)
                    {
                        sourceAddress = (void*)&objJson->valuedouble;
                    }
                    if (MirrorTypes::m_char == objTypeInfo->enumType ||
                        MirrorTypes::eKeys == objTypeInfo->enumType)
                    {
                        sourceAddress = (void*)&objJson->valuestring[0];
                    }
                    memcpy(fieldAddress, sourceAddress, objTypeInfo->size);
                }
                else
                {
                    LOG_WARN("{0} Unsupported Type!", __FUNCTION__);
                }
                break;
            }
        }

        void local_DeserializeClass(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj)
        {
            if (objTypeInfo->superTypeInfo)
            {
                // #TODO Should super constructor be called?
                local_DeserializeClass(objJson, objTypeInfo->superTypeInfo, obj);
            }

            if (objTypeInfo->typeConstructorFunc)
            {
                objTypeInfo->typeConstructorFunc(obj);
            }

            if (objTypeInfo->fields.empty())
                return;

            const cJSON* iterator = objJson->child;
            while (iterator)
            {
                for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
                {
                    const Mirror::Field& field = objTypeInfo->fields[i];

                    if (strcmp(field.name.c_str(), iterator->string) != 0 ||
                        field.serializationFlags & Mirror::FieldSerializationFlags::_InspectorOnly)
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

            const Mirror::TypeInfo* const elementFirstTypeInfo = objTypeInfo->collectionTypeInfoFirst;
            Buffer elementFirstBuffer(elementFirstTypeInfo->size);

            const cJSON* iterator = objJson->child;
            size_t index = 0; // #TODO Scrap index for non-contiguous collections if possible
            while (iterator)
            {
                std::unordered_map<GUID, Editor::EditorWindow*>* window = (std::unordered_map<GUID, Editor::EditorWindow*>*)obj;
                FromJson(iterator, elementFirstTypeInfo, elementFirstBuffer.As<void>());
                objTypeInfo->CollectionAppend(obj, index, elementFirstBuffer.As<void>(), nullptr);
                iterator = iterator->next;
                ++index;
            }
        }

        void local_DeserializePair(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson || objTypeInfo->category != Mirror::TypeInfoCategory_Pair)
            {
                LOG_ERROR("{0} Invalid argument passed!", __FUNCTION__);
                return;
            }

            const Mirror::TypeInfo* const elementFirstTypeInfo = objTypeInfo->collectionTypeInfoFirst;
            Buffer elementFirstBuffer(elementFirstTypeInfo->size);

            const Mirror::TypeInfo* const elementSecondInfo = objTypeInfo->collectionTypeInfoSecond;
            Buffer elementSecondBuffer(elementSecondInfo ? elementSecondInfo->size : 0);

            FromJson(objJson->child, elementFirstTypeInfo, elementFirstBuffer.As<void>());

            const cJSON* secondJson = objJson->child->next;
            if (Mirror::TypeInfoCategory_Primitive == elementSecondInfo->category)
            {   // #TODO Review handling primitives differently
                secondJson = secondJson->child;
            }
            FromJson(secondJson, elementSecondInfo, elementSecondBuffer.As<void>());

            objTypeInfo->CollectionAppend(obj, 0, elementFirstBuffer.As<void>(), elementSecondBuffer.As<void>());
        }

    }

}
