#include "QF_Serialization.h"

#include <unordered_map>

#include "QC_Guid.h"

#include "QF_Buffer.h"
#include "QF_ComponentHelpers.h"
#include "QF_EntityHandle.h"
#include "QF_Scene.h"

namespace QwerkE {

    namespace Serialization {

        void local_DeserializePrimitive(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, bool useNameString = false);
        void local_DeserializeClass(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj);
        void local_DeserializeCollection(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, const std::string& name);

        template<typename... Component>
        void NewDeserializeComponent(EntityHandle& handle, const cJSON* entityComponentsJsonArray)
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
        static void NewDeserializeComponents(TemplateArgumentList<Component...>, EntityHandle& handle, const cJSON* entityComponentsJsonArray)
        {
            NewDeserializeComponent<Component...>(handle, entityComponentsJsonArray);
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

                        NewDeserializeComponents(EntityComponentsList{}, handle, iteratorComponents);

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

        void DeserializeFromJson(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj)
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

                    const Mirror::TypeInfo* dereferencedTypeInfo = objTypeInfo->pointerDereferencedTypeInfo;

                    if (dereferencedTypeInfo->hasSubClass() &&
                        strcmp(dereferencedTypeInfo->stringName.c_str(), objJson->child->string) != 0)
                    {
                        if (dereferencedTypeInfo->isAbstract) {}

                        for (const auto& pair : dereferencedTypeInfo->derivedTypesMap)
                        {
                            if (strcmp(pair.second->stringName.c_str(), objJson->child->string) == 0)
                            {
                                dereferencedTypeInfo = pair.second;
                                break;
                            }
                        }
                    }

                    void* derefencedTypeObjAddress = *(void**)obj;
                    derefencedTypeObjAddress = new char[dereferencedTypeInfo->size];
                    *(void**)obj = derefencedTypeObjAddress;
                    DeserializeFromJson(objJson->child, dereferencedTypeInfo, derefencedTypeObjAddress);

                    // #EXPERIMENTAL
                    const bool constructorHasDependency = nullptr != dereferencedTypeInfo->typeConstructorDependentFunc;
                    if (constructorHasDependency)
                    {
                        void* buffer = new char[dereferencedTypeInfo->sizeOfConstructorArgumentBuffer];
                        buffer; // #TODO Fill buffer with argument data. Might need to parse json structure for keys
                        dereferencedTypeInfo->typeConstructorDependentFunc(derefencedTypeObjAddress, buffer);
                    }
                    else
                    {
                        dereferencedTypeInfo->typeConstructorFunc(derefencedTypeObjAddress);
                    }
                }
                break;

            case Mirror::TypeInfoCategories::TypeInfoCategory_Invalid:
            default:
                ASSERT(false, "Unsupported category!");
                break;
            }
        }

        void local_DeserializePrimitive(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, bool isPairFirst)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            switch (objTypeInfo->enumType)
            {
            case MirrorTypes::m_string: // #NOTE Handle when obj is pair.first
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

        void local_DeserializeClass(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj)
        {
            if (objTypeInfo->superTypeInfo)
            {
                local_DeserializeClass(objJson, objTypeInfo->superTypeInfo, obj);
            }

            const cJSON* iterator = objJson->child;
            while (iterator)
            {
                for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
                {
                    const Mirror::Field& field = objTypeInfo->fields[i];

                    if (strcmp(field.name.c_str(), iterator->string) != 0 ||
                        field.serializationFlags & Mirror::FieldSerializationFlags::_InspectorOnly)
                        continue;

                    if (field.typeInfo->isCollection()) // #NOTE Needed to pass name of member collection
                    {
                        local_DeserializeCollection(iterator, field.typeInfo, (char*)obj + field.offset, field.name);
                    }
                    else
                    {
                        DeserializeFromJson(iterator, field.typeInfo, (char*)obj + field.offset);
                    }
                }

                iterator = iterator->next;
            }
        }

        void local_DeserializeCollection(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            const Mirror::TypeInfo* const elementFirstTypeInfo = objTypeInfo->CollectionTypeInfoFirst();
            Buffer elementFirstBuffer(elementFirstTypeInfo->size);

            const Mirror::TypeInfo* const elementSecondInfo = objTypeInfo->CollectionTypeInfoSecond();
            Buffer elementSecondBuffer(elementSecondInfo ? elementSecondInfo->size : 0);

            const cJSON* iterator = objJson->child;
            size_t index = 0;
            while (iterator)
            {
                const bool hasConstructionDependency = false;
                // #TODO Handle case where string needs construction, but not basic/primitive types
                if (!hasConstructionDependency &&
                    (!elementFirstTypeInfo->isPrimitive() || MirrorTypes::m_string == elementFirstTypeInfo->enumType))
                {
                    ASSERT(elementFirstTypeInfo->typeConstructorFunc, "Construct function is null!");
                    elementFirstTypeInfo->typeConstructorFunc(elementFirstBuffer.As<void>());
                }

                DeserializeFromJson(iterator, elementFirstTypeInfo, elementFirstBuffer.As<void>());

                if (objTypeInfo->isPair())
                {
                    DeserializeFromJson(iterator, elementSecondInfo, elementSecondBuffer.As<void>());
                }

                objTypeInfo->CollectionAppend(obj, index, elementFirstBuffer.As<void>(), elementSecondBuffer.As<void>());

                if (hasConstructionDependency &&
                    (!elementFirstTypeInfo->isPrimitive() || MirrorTypes::m_string == elementFirstTypeInfo->enumType))
                {
                    ASSERT(elementFirstTypeInfo->typeConstructorFunc, "Construct function is null!");
                    elementFirstTypeInfo->typeConstructorFunc(elementFirstBuffer.As<void>()); // #TODO Add/supply constructor argument data
                }

                iterator = iterator->next;
                ++index;
            }
        }

    }

}
