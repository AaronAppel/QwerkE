#include "QF_Serialization.h"

#include <map>
#include <vector>

#include "Libraries/cJSON/QC_cJSON.h"
#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_eKeys.h"
#include "QF_GameObject.h"
#include "QF_Log.h"
#include "QF_Material.h"
#include "QF_Assets.h"
#include "QF_Settings.h"

#include "QF_RenderRoutine.h"
#include "QF_Routine.h"
#include "QF_TransformRoutine.h"

#include "QF_CameraComponent.h"
#include "QF_LightComponent.h"
#include "QF_RenderComponent.h"

#define no_file "None" // #TODO Consider coupling to cJSON logic and moving out of here

#define SERIALIZER_OPTIMIZATION_LEVEL 0 // Unsafe if serialization pattern changed, but much less redundant checking

// #TODO Look at removing from QwerkE namespace, or have a #using QwerkE just in this .cpp.
// Helps with porting to a stand alone library, and encourages type indifference/independence
namespace QwerkE {

    namespace Serialization {

        void DeserializeJsonArray(const cJSON* jsonObj, const Mirror::Field& field, void* obj);
        void DeserializeJsonNumber(const cJSON* jsonObj, const MirrorTypes type, const unsigned int size, void* obj);
        void DeserializeJsonString(const cJSON* jsonObj, const MirrorTypes type, void* obj);

        void DeserializeJsonToObject(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj)
        {
            if (!objJson || !objTypeInfo || !obj)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            std::vector<cJSON*> jsonStructArr = GetAllItemsFromArray(objJson); // #TODO Walk the linked list instead of creating a new, temp vector?
            if (jsonStructArr.empty() && objJson->type != cJSON_Array)
            {
                jsonStructArr = GetAllSiblingsWithObject(objJson);
            }

            if (jsonStructArr.size() != objTypeInfo->fields.size())
            {
                LOG_WARN("{0} Mismatched array sizes for json object {1} ", __FUNCTION__, objJson->string);
            }

            if (objTypeInfo->IsPrimitive())
            {
                switch (objJson->type)
                {
                case cJSON_String:
                    DeserializeJsonString(objJson, objTypeInfo->enumType, (char*)obj);
                    break;

                case cJSON_True:
                case cJSON_False:
                case cJSON_Number:
                    DeserializeJsonNumber(objJson, objTypeInfo->enumType, objTypeInfo->size, (char*)obj);
                    break;

                default:
                    // #TODO Handle reading/writing a simple primitive to a file
                    LOG_WARN("{0} Unhandled early return for type {1}", __FUNCTION__, (int)objTypeInfo->enumType);
                    return;
                }
            }

            for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
            {
                const Mirror::Field& field = objTypeInfo->fields[i];

#if SERIALIZER_OPTIMIZATION_LEVEL > 0
                for (size_t j = i; j < jsonStructArr.size(); j++)
                {
#else
                for (size_t j = 0; j < jsonStructArr.size(); j++)
                {
#endif
#if SERIALIZER_OPTIMIZATION_LEVEL < 2
                    if (strcmp(field.name.c_str(), jsonStructArr[j]->string) != 0)
                        continue;
#endif
                    switch (jsonStructArr[j]->type)
                    {
                    case cJSON_String:
                        DeserializeJsonString(jsonStructArr[j], field.typeInfo->enumType, (char*)obj + field.offset);
                        break;

                    case cJSON_True:
                    case cJSON_False:
                    case cJSON_Number:
                        DeserializeJsonNumber(jsonStructArr[j], field.typeInfo->enumType, field.typeInfo->size, (char*)obj + field.offset);
                        break;

                    case cJSON_Array:
                        DeserializeJsonArray(jsonStructArr[j], field, (char*)obj + field.offset);
                        break;

                    case cJSON_Object:
                    case cJSON_NULL:
                    default:
                        LOG_WARN("{0} JSON type {1} not supported here!", __FUNCTION__, (int)jsonStructArr[j]->type);
                        break;
                    }

                    break; // Next field
                }
            }
        }

        template <class T>
        T* DeserializeSubclassPointer(const cJSON* jsonObj) // #TODO Look to handle sub classes more elegantly
        {
            T* indexPtrAddress = nullptr;

            switch (Mirror::InfoForType<T>()->enumType)
            {
            case MirrorTypes::Routine:
                switch ((eRoutineTypes)std::stoi(jsonObj->string))
                {
                case eRoutineTypes::Routine_Render:
                    {
                        indexPtrAddress = new RenderRoutine();
                        DeserializeJsonToObject(jsonObj, Mirror::InfoForType<RenderRoutine>(), indexPtrAddress);
                    }
                    break;

                case eRoutineTypes::Routine_Transform:
                    {
                        indexPtrAddress = new TransformRoutine();
                        DeserializeJsonToObject(jsonObj, Mirror::InfoForType<TransformRoutine>(), indexPtrAddress);
                    }
                    break;
                }
                break;

            default:
                break;
            }

            // Deserialize parent/derived values
            // Mirror::GetParentClassInfo<Routine>()
            DeserializeJsonToObject(jsonObj, Mirror::InfoForType<Routine>(), indexPtrAddress);

            return indexPtrAddress;
        }

        template <class T>
        void DeserializeVector(const cJSON* jsonObj, void* obj)
        {
            typedef std::remove_pointer_t<T> AbsoluteTypeT;

            const std::vector<cJSON*> jsonObjectVector = GetAllItemsFromArray(jsonObj);

            if (std::is_pointer_v<T>)
            {
                std::vector<AbsoluteTypeT*>* vectorPtrPtr = (std::vector<AbsoluteTypeT*>*)obj; // #TODO Review if possible to validate pointer value
                vectorPtrPtr->reserve(jsonObjectVector.size());
                vectorPtrPtr->resize(0);

                for (size_t i = 0; i < jsonObjectVector.size(); i++)
                {
                    AbsoluteTypeT* objectPtr = new AbsoluteTypeT();

                    DeserializeJsonToObject(jsonObjectVector[i], Mirror::InfoForType<AbsoluteTypeT>(), objectPtr);
                    vectorPtrPtr->push_back(objectPtr);
                }
            }
            else
            {
                std::vector<AbsoluteTypeT>* vectorPtrPtr = (std::vector<AbsoluteTypeT>*)obj; // #TODO Review if possible to validate pointer value
                vectorPtrPtr->reserve(jsonObjectVector.size());
                vectorPtrPtr->resize(0);

                for (size_t i = 0; i < jsonObjectVector.size(); i++)
                {
                    AbsoluteTypeT objectInstance;
                    DeserializeJsonToObject(jsonObjectVector[i], Mirror::InfoForType<AbsoluteTypeT>(), &objectInstance);
                    vectorPtrPtr->push_back(objectInstance);
                }
            }
        }

        void DeserializeJsonArray(const cJSON* jsonObj, const Mirror::Field& field, void* obj)
        {
            if (!jsonObj || !jsonObj->type || jsonObj->type != cJSON_Array || !obj)
            {
                LOG_ERROR("{0} Null/invalid argument passed!", __FUNCTION__);
                return;
            }

            switch (field.typeInfo->enumType)
            {
            case MirrorTypes::m_vector_string:
                // {
                //     // DeserializeVector<std::string>(jsonObj, obj); // #TODO Find out why std::string type is not supported
                //     // return;
                //
                //     std::vector<std::string>* strings = (std::vector<std::string>*)((char*)obj);
                //     const std::vector<cJSON*> jsonStrings = GetAllItemsFromArray(jsonObj);
                //     if (strings)
                //     {
                //         strings->reserve(jsonStrings.size());
                //         strings->resize(0);
                //
                //         for (size_t i = 0; i < jsonStrings.size(); i++)
                //         {
                //             // #TODO Handle more types agnosticly, so this logic would also support ints and other primitives/base values
                //             // #TODO This could be templated using the MirrorTypes enum if the function handles more than strings
                //             // DeserializeJsonString(jsonObj, MirrorTypes::m_string, obj);
                //
                //             strings->emplace_back(jsonStrings[i]->valuestring);
                //         }
                //     }
                // }
                DeserializeVector<std::string>(jsonObj, obj);
                break;

            case MirrorTypes::m_vector_renderable:
                DeserializeVector<Renderable>(jsonObj, obj);
                break;

            case MirrorTypes::m_vector_gameobjectPtr:
                DeserializeVector<GameObject*>(jsonObj, obj);
                break;

            case MirrorTypes::m_vector_routinePtr:
            {
                // DeserializeVector<Routine*>(jsonObj, obj); // #TODO Handle sub class type construction
                // return;

                if (auto collectionInfo = field.typeInfo->collectionTypeInfo)
                {
                    if (!collectionInfo->derivedTypesMap.empty())
                    {
                        auto map = collectionInfo->derivedTypesMap;
                        for (auto& currentRoutine : map)
                        {
                            if (false)
                            {
                                // #TODO Determine which sub class currentRoutine is
                                Routine* newRoutine = nullptr;
                                newRoutine = new RenderRoutine();
                            }
                        }
                    }
                }

                std::vector<Routine*>* routines = (std::vector<Routine*>*)obj;
                const std::vector<cJSON*> jsonGameObjects = GetAllItemsFromArray(jsonObj);
                if (routines) // #TODO Validate null pointer value
                {
                    routines->reserve(jsonGameObjects.size());
                    routines->resize(0);

                    for (size_t i = 0; i < jsonGameObjects.size(); i++)
                    {
                        Routine* indexPtrAddress = (Routine*)routines->data() + i;
                        indexPtrAddress = DeserializeSubclassPointer<Routine>(jsonGameObjects[i]);

                        if (indexPtrAddress)
                        {
                            routines->push_back(indexPtrAddress);
                        }
                    }
                }
            }
            break;

            case MirrorTypes::m_map_eComponentTags_componentPtr: // #TODO Add parent and sub class serialization/reflection support
            {
                std::map<eComponentTags, Component*>* componentsMap = (std::map<eComponentTags, Component*>*)obj;
                const std::vector<cJSON*> jsonGameObjects = GetAllItemsFromArray(jsonObj);
                if (componentsMap)
                {
                    if (auto collectionInfo = field.typeInfo->collectionTypeInfo)
                    {
                        if (!collectionInfo->derivedTypesMap.empty())
                        {
                            for (size_t i = 0; i < jsonGameObjects.size(); i++)
                            {
                                int bp = 0;
                                // Component* component = nullptr; // #TODO component = (Component*)collectionInfo->CreateSubClass(enumType);
                                // DeserializeJsonToObject(jsonGameObjects[i], Mirror::InfoForType<ComponentCamera>(), component);
                            }
                        }
                    }

                    componentsMap->clear();

                    Component* component = nullptr;
                    for (size_t i = 0; i < jsonGameObjects.size(); i++)
                    {
                        eComponentTags componentTag = (eComponentTags)std::stoi(jsonGameObjects[i]->string);
                        if (componentsMap->find(componentTag) != componentsMap->end())
                        {
                            LOG_ERROR("{0} Found duplicated key {1} in map!", __FUNCTION__, (int)componentTag);
                            continue;
                        }

                        switch (componentTag)
                        {
                        case Component_Camera:
                            component = new ComponentCamera();
                            DeserializeJsonToObject(jsonGameObjects[i], Mirror::InfoForType<ComponentCamera>(), component);
                            break;

                        case Component_Light:
                            component = new LightComponent();
                            DeserializeJsonToObject(jsonGameObjects[i], Mirror::InfoForType<LightComponent>(), component);
                            ((LightComponent*)component)->SetColour(vec3(1.0f, 1.0f, 1.0f));
                            break;

                        case Component_Render:
                            component = new RenderComponent();
                            DeserializeJsonToObject(jsonGameObjects[i], Mirror::InfoForType<RenderComponent>(), component);
                            break;

                        case Component_Physics:
                        case Component_Controller:
                        case Component_SkyBox:
                        case Component_SoundPlayer:
                        case Component_SoundListener:
                        case Component_Max:
                        case Component_Null:
                        default:
                            LOG_WARN("{0} TODO", __FUNCTION__);
                            break;
                        }

                        if (component)
                        {
                            std::map<eComponentTags, Component*>& componentMapRef = *componentsMap; // #TODO BUG Look why map->insert({}) wasn't working with initializer list
                            componentMapRef[componentTag] = component;
                        }
                    }
                }
            }
            break;

            case MirrorTypes::m_map_eMaterialMaps_texturePtr: // #TODO Textures should be created and requested by Resources
                {
                    std::map<eMaterialMaps, Texture*>* texturesMap = (std::map<eMaterialMaps, Texture*>*)obj;
                    const std::vector<cJSON*> jsonGameObjects = GetAllItemsFromArray(jsonObj);
                    if (texturesMap)
                    {
                        texturesMap->clear();

                        for (size_t i = 0; i < jsonGameObjects.size(); i++)
                        {
                            eMaterialMaps componentTag = eMaterialMaps::_from_index(std::stoi(jsonGameObjects[i]->string));
                            if (texturesMap->find(componentTag) != texturesMap->end())
                            {
                                LOG_ERROR("{0} Found duplicated key {1} in map!", __FUNCTION__, (int)componentTag);
                                continue;
                            }

                            Texture texture;
                            DeserializeJsonToObject(jsonGameObjects[i], Mirror::InfoForType<Texture>(), &texture);
                            Texture* result = Assets::GetTexture(texture.s_FileName.c_str());

                            std::map<eMaterialMaps, Texture*>& componentMapRef = *texturesMap; // #TODO BUG Look why map->insert({}) wasn't working with initializer list
                            componentMapRef[componentTag] = result;
                            continue;
                        }
                    }
                }
                break;

            default:
                DeserializeJsonToObject(jsonObj, field.typeInfo, obj);
                break;
            }
        }

        void DeserializeJsonNumber(const cJSON* jsonObj, const MirrorTypes type, const unsigned int typeSize, void* obj)
        {
            if (!jsonObj || !obj)
            {
                LOG_ERROR("{0} Null/invalid argument passed!", __FUNCTION__);
                return;
            }

            if (jsonObj->type == cJSON_False && jsonObj->type == cJSON_True && jsonObj->type == cJSON_Number)
            {
                LOG_ERROR("{0} JSON object type is not a number or boolean!", __FUNCTION__);
                return;
            }

            switch (jsonObj->type)
            {
            case cJSON_True:
            case cJSON_False:
                {
                    bool* fieldAddress = ((bool*)obj);
                    *fieldAddress = jsonObj->type == cJSON_True;
                }
                break;

            case cJSON_Number:
                {
                    void* fieldAddress = ((char*)obj);

                    switch (type)
                    {
                    case MirrorTypes::m_float:
                        {
                            float* floatValuePtr = (float*)fieldAddress;
                            *floatValuePtr = (float)jsonObj->valuedouble;
                        }
                        break;

                    case MirrorTypes::m_double:
                        {
                            double* doubleValuePtr = (double*)fieldAddress;
                            *doubleValuePtr = (double)jsonObj->valuedouble;
                        }
                        break;

                    default:
                        void* sourceAddress = (void*)&jsonObj->valueint;
                        memcpy(fieldAddress, sourceAddress, sizeof(jsonObj->valueint));
                        break;
                    }
                }
                break;

            case cJSON_Array:
            case cJSON_Object:
            case cJSON_NULL:
            default:
                // LOG_WARN("{0} JSON type {1} not supported here!", __FUNCTION__, (int)jsonStructArr[j]->type);
                LOG_WARN("Erorr in {0}", __FUNCTION__);
                break;
            }
        }

        void DeserializeJsonString(const cJSON* jsonObj, const MirrorTypes type, void* obj)
        {
            if (!jsonObj || !jsonObj->valuestring || (jsonObj->type != cJSON_String) || !obj)
            {
                LOG_ERROR("{0} Null/invalid argument passed!", __FUNCTION__);
                return;
            }

            switch (type)
            {
            case MirrorTypes::m_string:
                {
                    std::string* fieldAddress = (std::string*)((char*)obj);
                    *fieldAddress = jsonObj->valuestring;
                }
                break;

            case MirrorTypes::eKeys:
            case MirrorTypes::m_char:
                {
                    char* fieldAddress = (char*)obj;
                    *fieldAddress = jsonObj->valuestring[0];
                }
                break;

            case MirrorTypes::m_charPtr:
            case MirrorTypes::m_constCharPtr:
                {
                    char** fieldAddress = (char**)((char*)obj);
                    *fieldAddress = (char*)_strdup(jsonObj->valuestring); // #TODO Handle malloc with matching free
                }
                break;

            default:
                // LOG_ERROR("{0} Unsupported field type {1} {2}({3}) for cJSON deserialization!", __FUNCTION__, field.name, field.type->stringName, (int)field.type->enumType);
                LOG_ERROR("Erorr in {0}", __FUNCTION__);
                break;
            }
        }

        template <class T>
        void SerializeVector(void* obj, cJSON* objJson)
        {
            typedef std::remove_pointer_t<T> AbsoluteTypeT;

            const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<AbsoluteTypeT>();

            if (std::is_pointer_v<T>)
            {
                const std::vector<AbsoluteTypeT*>* vectorPtrPtr = (std::vector<AbsoluteTypeT*>*)obj; // #TODO Review if possible to validate pointer value

                for (size_t i = 0; i < vectorPtrPtr->size(); i++)
                {
                    if (!vectorPtrPtr->at(i))
                        continue;

                    cJSON* newJsonObjectArray = CreateArray(typeInfo->stringName.c_str());
                    SerializeObjectToJson(vectorPtrPtr->at(i), typeInfo, newJsonObjectArray);
                    AddItemToArray(objJson, newJsonObjectArray);
                }
            }
            else
            {
                const std::vector<AbsoluteTypeT>* vectorPtr = (std::vector<AbsoluteTypeT>*)obj; // #TODO Review if possible to validate pointer value

                for (size_t i = 0; i < vectorPtr->size(); i++)
                {
                    cJSON* newJsonObjectArray = CreateArray(typeInfo->stringName.c_str());
                    SerializeObjectToJson(&vectorPtr->at(i), typeInfo, newJsonObjectArray);
                    AddItemToArray(objJson, newJsonObjectArray);
                }
            }
        }

        void SerializeObjectToJson(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson)
        {
            if (!objJson || !objTypeInfo || !obj)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            // Could consider trying switch statement 1st, and then handling non-explicitly supported/complex types in the default case
            // if (!objTypeInfo->fields.empty())
            // {
            //     for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
            //     {
            //         const Mirror::Field& field = objTypeInfo->fields[i];
            //
            //         cJSON* arr = nullptr;
            //         if (!field.typeInfo->IsPrimitive())
            //         {
            //             arr = CreateArray(field.name.c_str());
            //             cJSON_AddItemToArray(objJson->child, arr);
            //         }
            //         else
            //         {
            //             arr = objJson;
            //         }
            //
            //         SerializeObjectToJson((char*)obj + field.offset, field.typeInfo, arr);
            //     }
            //     return;
            // }

            if (objTypeInfo->enumType == MirrorTypes::Vector3)
            {
                int bp = 0;
            }

            for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
            {
                const Mirror::Field& field = objTypeInfo->fields[i];

                cJSON* arr = nullptr;
                if (!field.typeInfo->IsPrimitive())
                {
                    arr = CreateArray(field.name.c_str());
                    cJSON_AddItemToArray(objJson->child, arr);
                }

                // if (field.classInfo)
                // {
                //     SerializeObjectToJson((char*)obj + field.offset, field.typeInfo, arr);
                //     continue;
                // }

                switch (field.typeInfo->enumType)
                {
                case MirrorTypes::m_vector_string: // #TODO Try to remove this case to use if (!objTypeInfo->fields.empty()) for loop
                    {
                        const std::vector<std::string>* strings = (std::vector<std::string>*)((char*)obj + field.offset);
                        if (strings)
                        {
                            for (size_t i = 0; i < strings->size(); i++)
                            {
                                std::string str = strings->at(i);
                                AddItemToArray(arr, CreateString(std::to_string(i).c_str(), str.c_str()));
                            }
                        }
                        else
                        {
                            LOG_WARN("{0} Unable to deserialize type of {1} {2}({3})", __FUNCTION__, field.name, field.typeInfo->stringName, (int)field.typeInfo->enumType);
                        }
                    }
                    break;

                case MirrorTypes::m_vector_routinePtr:
                {
                    const std::vector<Routine*>* routines = (std::vector<Routine*>*)((char*)obj + field.offset);
                    if (routines)
                    {
                        if (!field.typeInfo->derivedTypesMap.empty())
                        {
                            auto map = field.typeInfo->derivedTypesMap;
                            for (auto& it : map)
                            {
                                // Determine which sub class
                            }
                        }

                        for (size_t i = 0; i < routines->size(); i++)
                        {
                            Routine* routine = routines->at(i);
                            cJSON* routineJson = CreateArray(std::to_string((int)routine->GetRoutineType()).c_str());

                            // Serialize parent class values
                            SerializeObjectToJson(routine, Mirror::InfoForType<Routine>(), routineJson);

                            switch (routine->GetRoutineType())
                            {
                            case eRoutineTypes::Routine_Render:
                                SerializeObjectToJson(routine, Mirror::InfoForType<RenderRoutine>(), routineJson);
                                // SerializeObjectToJson(routine, Mirror::GetParentClassInfo<RenderRoutine>(), routineJson);
                                break;

                            case eRoutineTypes::Routine_Transform:
                                SerializeObjectToJson(routine, Mirror::InfoForType<TransformRoutine>(), routineJson);
                                break;

                            default:
                                LOG_WARN("{0} Unsupported routine type {1}!", __FUNCTION__, routine->GetRoutineType());
                                break;
                            }
                            AddItemToArray(arr, routineJson);
                        }
                    }
                }
                break;

                case MirrorTypes::m_vector_renderable: // #TODO Needs T* CastToType() method for indirection
                    SerializeVector<Renderable>((void*)((char*)obj + field.offset), arr);
                    break;

                case MirrorTypes::m_vector_gameobjectPtr:
                    SerializeVector<GameObject*>((void*)((char*)obj + field.offset), arr);
                    break;

                case MirrorTypes::m_map_eComponentTags_componentPtr:
                    {
                        const std::map<eComponentTags, Component*>& components = *(std::map<eComponentTags, Component*>*)((char*)obj + field.offset);

                        for (auto& pair : components)
                        {
                            Component* component = pair.second;

                            if (!component)
                                continue;

                            eComponentTags tag = pair.first; // #TODO Review using map key

                            cJSON* t_Component = CreateArray(std::to_string(component->GetTag()).c_str());

                            switch (component->GetTag()) // #TODO Look to handle sub classes more elegantly
                            {
                            case Component_Render:
                                SerializeObjectToJson(component, Mirror::InfoForType<RenderComponent>(), t_Component);
                                break;

                            case Component_Camera:
                                SerializeObjectToJson(component, Mirror::InfoForType<ComponentCamera>(), t_Component);
                                break;

                            case Component_Light:
                            case Component_Physics:
                            case Component_Controller:
                            case Component_SkyBox:
                                // #TODO Implement component add
                                break;
                            }
                            AddItemToArray(arr, t_Component);
                        }
                    }
                    break;

                case MirrorTypes::m_map_eMaterialMaps_texturePtr: // #TODO Textures should be created and requested by Resources
                    {
                        const std::map<eMaterialMaps, Texture*>& components = *(std::map<eMaterialMaps, Texture*>*)((char*)obj + field.offset);

                        for (auto& pair : components)
                        {
                            Texture* texture = pair.second;

                            if (!texture)
                                continue;

                            cJSON* textureJson = CreateArray(std::to_string((int)pair.first).c_str());

                            // #TODO Use field.typeInfo->collectionTypeInfo instead of Mirror::InfoForType<Texture>()
                            SerializeObjectToJson(texture, Mirror::InfoForType<Texture>(), textureJson);

                            AddItemToArray(arr, textureJson);
                        }
                    }
                    break;

                case MirrorTypes::m_string:
                    {
                        const std::string* fieldAddress = (const std::string*)((char*)obj + field.offset);
                        AddItemToArray(objJson, CreateString(field.name.c_str(), fieldAddress->data()));
                    }
                    break;

                case MirrorTypes::m_charPtr:
                case MirrorTypes::m_constCharPtr:
                    {
                        const char* fieldAddress = *(const char**)((char*)obj + field.offset);
                        AddItemToArray(objJson, CreateString(field.name.c_str(), fieldAddress));
                    }
                    break;

                case MirrorTypes::eKeys:
                case MirrorTypes::m_char:
                    {
                        char* stringAddress = (char*)obj + field.offset;
                        AddItemToArray(objJson, CreateString(field.name.c_str(), stringAddress));
                    }
                    break;

                case MirrorTypes::m_bool:
                    {
                        bool* boolAddress = (bool*)((char*)obj + field.offset);
                        AddItemToArray(objJson, CreateBool(field.name.c_str(), *boolAddress));
                    }
                    break;

                case MirrorTypes::m_float:
                    {
                        float* numberAddress = (float*)((char*)obj + field.offset);
                        AddItemToArray(objJson, CreateNumber(field.name.c_str(), *numberAddress));
                    }
                    break;

                case MirrorTypes::m_eSceneTypes:
                case MirrorTypes::m_eGameObjectTags:
                case MirrorTypes::m_uint8_t:
                    {
                        uint8_t* numberAddress = (uint8_t*)((char*)obj + field.offset);
                        AddItemToArray(objJson, CreateNumber(field.name.c_str(), *numberAddress));
                    }
                    break;

                case MirrorTypes::m_uint16_t:
                    {
                        uint16_t* numberAddress = (uint16_t*)((char*)obj + field.offset);
                        AddItemToArray(objJson, CreateNumber(field.name.c_str(), *numberAddress));
                    }
                    break;

                case MirrorTypes::m_uint32_t:
                    {
                        uint32_t* numberAddress = (uint32_t*)((char*)obj + field.offset);
                        AddItemToArray(objJson, CreateNumber(field.name.c_str(), *numberAddress));
                    }
                    break;

                case MirrorTypes::m_int8_t:
                    {
                        int8_t* numberAddress = (int8_t*)((char*)obj + field.offset);
                        AddItemToArray(objJson, CreateNumber(field.name.c_str(), (double)*numberAddress));
                    }
                    break;

                case MirrorTypes::m_int16_t:
                    {
                        int16_t* numberAddress = (int16_t*)((char*)obj + field.offset);
                        AddItemToArray(objJson, CreateNumber(field.name.c_str(), *numberAddress));
                    }
                    break;

                case MirrorTypes::m_int:
                case MirrorTypes::m_int32_t:
                    {
                        int32_t* numberAddress = (int32_t*)((char*)obj + field.offset);
                        AddItemToArray(objJson, CreateNumber(field.name.c_str(), *numberAddress));
                    }
                    break;

                case MirrorTypes::m_int64_t:
                    {
                        int64_t* numberAddress = (int64_t*)((char*)obj + field.offset);
                        AddItemToArray(objJson, CreateNumber(field.name.c_str(), (double)*numberAddress));
                    }
                    break;

                case MirrorTypes::m_uint64_t:
                    {
                        uint64_t* numberAddress = (uint64_t*)((char*)obj + field.offset);
                        AddItemToArray(objJson, CreateNumber(field.name.c_str(), (double)*numberAddress));
                    }
                    break;

                case MirrorTypes::m_double:
                    {
                        double* numberAddress = (double*)((char*)obj + field.offset);
                        AddItemToArray(objJson, CreateNumber(field.name.c_str(), *numberAddress));
                    }
                    break;

                default:

                    if (!field.typeInfo->fields.empty())
                    {
                        SerializeObjectToJson((char*)obj + field.offset, field.typeInfo, arr);
                    }
                    else
                    {
                        LOG_WARN("{0} Unsupported user defined field type {1} {2}({3}) for serialization!", __FUNCTION__, field.name, field.typeInfo->stringName, (int)field.typeInfo->enumType);
                        break;
                    }
                }
            }
        }

    }

}