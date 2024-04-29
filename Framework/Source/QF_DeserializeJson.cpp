#include "QF_Serialization.h"

#include <unordered_map>

#include "QC_Guid.h"

#include "QF_Buffer.h"
#include "QF_ComponentHelpers.h"
#include "QF_EntityHandle.h"
#include "QF_Scene.h"

namespace QwerkE {

    namespace Serialization {

#define DEBUG_LEVEL 1

        void local_DeserializePrimitive(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, bool useNameString = false);
        void local_DeserializeClass(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj);
        void local_DeserializeCollection(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, const std::string& name);

        template <typename T>
        void DeserializeComponent(EntityHandle handle, const cJSON* componentJson, bool addComponent)
        {
            const Mirror::TypeInfo* componentTypeInfo = Mirror::InfoForType<T>();
            if (strcmp(componentJson->string, componentTypeInfo->stringName.c_str()) != 0)
                return;

            if (addComponent)
            {
                handle.AddComponent<T>();
            }

            T& component = handle.GetComponent<T>();
            DeserializeFromJson(componentJson, componentTypeInfo, (void*)&component);

            if (std::is_same_v<T, ComponentMesh>)
            {
                ComponentMesh* mesh = (ComponentMesh*)&component;
                mesh->Initialize();
            }
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
                    std::unordered_map<GUID, entt::entity>* entitiesMap = (std::unordered_map<GUID, entt::entity>*)obj;
                    entitiesMap->clear();

                    const s32 offSetOfEntitiesMap = OffsetOfMember(Mirror::InfoForType<Scene>(), "m_GuidsToEntts");
                    Scene* scene = (Scene*)((char*)obj - offSetOfEntitiesMap);

                    const cJSON* iteratorEntities = objJson->child;
                    size_t counterEntities = 0;

                    while (iteratorEntities)
                    {
                        GUID guid;
                        if (strcmp(iteratorEntities->string, "Entity") != 0)
                        {
                            guid = std::stoull(iteratorEntities->string);
                        }

                        EntityHandle handle = scene->CreateEntity(guid);

                        const cJSON* iteratorComponents = iteratorEntities->child;
                        size_t counterComponents = 0;

                        while (iteratorComponents)
                        {
                            // #TODO Look at using component enum instead of strings
                            // #TODO Serialize using EntityComponentsList
                            DeserializeComponent<ComponentTransform>(handle, iteratorComponents, false); // #NOTE Added to every entity on creation
                            DeserializeComponent<ComponentInfo>(handle, iteratorComponents, false);
                            DeserializeComponent<ComponentCamera>(handle, iteratorComponents, true); // #NOTE Need to be added only if in data
                            DeserializeComponent<ComponentMesh>(handle, iteratorComponents, true);
                            DeserializeComponent<ComponentScript>(handle, iteratorComponents, true); // #TODO Consider using ComponentScript.AddScript()

                            iteratorComponents = iteratorComponents->next;
                            ++counterComponents;
                        }

                        ASSERT(handle.HasComponent<ComponentTransform>(), "Entity must have ComponentTransform!");
                        ASSERT(handle.HasComponent<ComponentInfo>(), "Entity must have ComponentInfo!");

                        iteratorEntities = iteratorEntities->next;
                        ++counterEntities;
                    }

                    // for (size_t i = 0; i < entitiesJsonVector.size(); i++)
                    // {
                    //     const std::vector<cJSON*> componentsJsonVector = GetAllItemsFromArray(entitiesJsonVector[i]->child->child);
                    //
                    //     GUID guid;
                    //     if (strcmp(entitiesJsonVector[i]->string, "Entity") != 0)
                    //     {
                    //         guid = std::stoull(entitiesJsonVector[i]->string);
                    //     }
                    //
                    //     EntityHandle handle = scene->CreateEntity(guid);
                    //
                    //     for (size_t j = 0; j < componentsJsonVector.size(); j++)
                    //     {
                    //         // #TODO Look at using component enum instead of strings
                    //         DeserializeComponent<ComponentTransform>(handle, componentsJsonVector[j], false); // #NOTE Added to every entity on creation
                    //         DeserializeComponent<ComponentInfo>(handle, componentsJsonVector[j], false);
                    //         DeserializeComponent<ComponentCamera>(handle, componentsJsonVector[j], true); // #NOTE Need to be added only if in data
                    //         DeserializeComponent<ComponentMesh>(handle, componentsJsonVector[j], true);
                    //         DeserializeComponent<ComponentScript>(handle, componentsJsonVector[j], true); // #TODO Consider using ComponentScript.AddScript()
                    //     }
                    //
                    //     ASSERT(handle.HasComponent<ComponentTransform>(), "Entity must have ComponentTransform!");
                    //     ASSERT(handle.HasComponent<ComponentInfo>(), "Entity must have ComponentInfo!");
                    // }
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
            case Mirror::TypeInfoCategories::TypeInfoCategory_Pointer: // #TODO Look to remove
                {
                    // #TODO Handle constructor calls
                    *(void**)obj = new char[objTypeInfo->pointerDereferencedTypeInfo->size];
                    DeserializeFromJson(objJson->child, objTypeInfo->pointerDereferencedTypeInfo, *(void**)obj);
                }
                break;

            case Mirror::TypeInfoCategories::TypeInfoCategory_Invalid:
            default:
                ASSERT(false, "Unsupported category!");
                break;
            }
        }

        template <typename T, typename U>
        void local_Write(void* destination, const void* source) // #TODO Deprecate
        {
            // #NOTE Value of this function is in type conversion, rather than writing bytes of data that isn't in required format
            T* objAddress = (T*)destination;
            *objAddress = *(U*)source;
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
            case MirrorTypes::m_string:
                {
                    std::string* str = (std::string*)obj;
                    const char* result = isPairFirst ? objJson->string : objJson->valuestring;
                    if (!result) // #TODO Take out safety check with a better solution
                    {
                        result = isPairFirst ? objJson->valuestring : objJson->string;
                    }
                    *str = result;
                }
                break;
            case MirrorTypes::m_charPtr:
            case MirrorTypes::m_constCharPtr:
                {
                    const char** str = (const char**)obj;
                    *str = _strdup(objJson->valuestring);
                }
                break;

            case MirrorTypes::m_uint64_t: // #NOTE Storing uint64 as string
                {
                    u64* valuePtr = (u64*)obj;
                    if (!objJson->valuestring)
                    {
                        LOG_ERROR("{0} objJson->valuestring is null!", __FUNCTION__);
                        return;
                    }
                    const u64 temp = std::stoull(objJson->valuestring);
                    *valuePtr = temp;
                    // #TODO Try using objJson->valuedouble and a memcpy to see if that works
                }
                break;
            case MirrorTypes::m_int64_t: // #NOTE Storing int64 as string
                {
                    s64* valuePtr = (s64*)obj;
                    const s64 temp = std::stoll(objJson->valuestring);
                    *valuePtr = temp;
                    // #TODO Try using objJson->valuedouble and a memcpy to see if that works
                }
                break;

            case MirrorTypes::m_float:
                local_Write<float, double>(obj, &objJson->valuedouble); break;

            default:
                // if (cJSON_Number == objJson->type || cJSON_True == objJson->type || cJSON_False == objJson->type)
                if (objJson->type | cJSON_Number | cJSON_True | cJSON_False)
                {
                    // #TODO Potential bug writing to fieldAddress with size less than objJson->valueint
                    // #TODO Try to use memcpy for bool, float, double, and char assignments/cases
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
#if DEBUG_LEVEL 1
                else
                {
                    LOG_WARN("{0} Unsupported Type!", __FUNCTION__);
                }
#endif
                break;
            }
        }

        void local_DeserializeClass(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj)
        {
            const cJSON* iterator = objJson->child;

            if (objTypeInfo->superTypeInfo)
            {
                local_DeserializeClass(objJson, objTypeInfo->superTypeInfo, obj);
            }

            while (iterator)
            {
                for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
                {
                    const Mirror::Field& field = objTypeInfo->fields[i];

                    if (strcmp(field.name.c_str(), iterator->string) != 0)
                    {
#if DEBUG_LEVEL 1
                        // LOG_WARN("{0} Mismatched names {1} and {2}!", __FUNCTION__, field.name.c_str(), iterator->string);
#endif
                        continue;
                    }

                    if (field.serializationFlags & Mirror::FieldSerializationFlags::_InspectorOnly)
                        continue;

                    if (field.typeInfo->isCollection())
                    {
                        // #NOTE Needed to pass name
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

            const cJSON* it = objJson->child;
            size_t index = 0; // #TODO Use fields array in a for loop

            while (it)
            {
                const bool hasConstructionDependency = false;
                // #TODO Handle case where string needs construction, but not basic/primitive types
                if (!hasConstructionDependency &&
                    (!elementFirstTypeInfo->isPrimitive() || MirrorTypes::m_string == elementFirstTypeInfo->enumType))
                {
                    ASSERT(elementFirstTypeInfo->typeConstructorFunc, "Construct function is null!");
                    elementFirstTypeInfo->typeConstructorFunc(elementFirstBuffer.As<void>());
                }

                DeserializeFromJson(it, elementFirstTypeInfo, elementFirstBuffer.As<void>());

                if (objTypeInfo->isPair())
                {
                    DeserializeFromJson(it, elementSecondInfo, elementSecondBuffer.As<void>());
                }

                objTypeInfo->CollectionAppend(obj, index, elementFirstBuffer.As<void>(), elementSecondBuffer.As<void>());

                if (hasConstructionDependency &&
                    (!elementFirstTypeInfo->isPrimitive() || MirrorTypes::m_string == elementFirstTypeInfo->enumType))
                {
                    ASSERT(elementFirstTypeInfo->typeConstructorFunc, "Construct function is null!");
                    elementFirstTypeInfo->typeConstructorFunc(elementFirstBuffer.As<void>()); // #TODO Add/supply constructor argument data
                }

                it = it->next;
                ++index;
            }
        }

    }

}
