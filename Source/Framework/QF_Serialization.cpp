#include "QF_Serialization.h"

#include "Libraries/cJSON/QC_cJSON.h"

#include "QF_eKeys.h"
#include "QF_DataManager.h"
#include "QF_GameObject.h"
#include "QF_Log.h"
#include "QF_Resources.h"

#include "QF_RenderRoutine.h"
#include "QF_Routine.h"
#include "QF_TransformRoutine.h"

#include "QF_LightComponent.h"
#include "QF_CameraComponent.h"
#include "QF_FirstPersonCameraComponent.h"
#include "QF_FreeCameraComponent.h"
#include "QF_StaticCameraComponent.h"
#include "QF_ThirdPersonCameraComponent.h"

#define no_file "None" // #TODO Consider coupling to cJSON logic and moving out of here

// #TODO Review how arrays are handled. Are they left empty when they have no instances, and
// more specifically, are their child arrays left empty? May as well skip empty child arrays
// to reduce extra file data and parsing, unless a need arises for searching through countless
// empty arrays in a cJSON file tree structure.

// #TODO Look at removing from QwerkE namespace, or have a #using QwerkE just in this .cpp.
// Helps with porting to a stand alone library, and encourages type indifference/independence
namespace QwerkE {

    namespace Serialization {

        typedef TransformRoutine* m_transformRoutinePtr;

        // #TODO Create private (hidden from user) methods to handle individual reads and writes of types.
        // These methods can help reduce the indentation, and improve readability + debug-ability.

#define DESERIALIZE_CASE_FOR_CLASS(ClassType) \
case MirrorTypes::ClassType: \
    DeserializeJsonArrayToObject(jsonObj, Mirror::InfoForClass<ClassType>(), obj); \
    break; \

        void DeserializeJsonArrayToObject(const cJSON* objJson, const Mirror::ClassInfo* objClassInfo, void* obj)
        {
            if (!objJson || !objClassInfo || !obj)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (objJson->type != cJSON_Array)
            {
                LOG_ERROR("{0} Only takes a JSON array! Type was {1}", __FUNCTION__, (int)objJson->type);
                return;
            }

            const std::vector<cJSON*>& jsonStructArr = GetAllItemsFromArray(objJson);

            if (jsonStructArr.size() != objClassInfo->fields.size())
            {
                int breakPoint = 0;
                // LOG_WARN("{0} Mismatched array sizes for class {1} ", __FUNCTION__, Mirror::InfoForType(objClassInfo)->stringName.c_str());
                // LOG_WARN("{0} Mismatched array sizes for class {1} ", __FUNCTION__, objClassInfo->type->stringName.c_str());
            }

            for (size_t i = 0; i < objClassInfo->fields.size(); i++)
            {
                const Mirror::Field& field = objClassInfo->fields[i];

                // #TODO Consider starting j at i or index something higher than 0.
                // Could rely on serialization order and use i, but not the safest option.
                for (size_t j = 0; j < jsonStructArr.size(); j++)
                {
                    // #TODO Add breaks and/or continues to reduce extra iterations

                    if (strcmp(field.name.c_str(), jsonStructArr[j]->string) != 0)
                        continue;

                    bool shouldBreakForLoop = false;
                    switch (jsonStructArr[j]->type)
                    {
                    case cJSON_String:
                        DeserializeJsonString(jsonStructArr[j], field.type->enumType, (char*)obj + field.offset);
                        shouldBreakForLoop = true;
                        break;

                    case cJSON_True:
                    case cJSON_False:
                    case cJSON_Number:
                        DeserializeJsonNumber(jsonStructArr[j], field.type->enumType, field.type->size, (char*)obj + field.offset);
                        shouldBreakForLoop = true;
                        break;

                    case cJSON_Array:
                        DeserializeJsonArray(jsonStructArr[j], field, (char*)obj + field.offset);
                        shouldBreakForLoop = true;
                        break;

                    case cJSON_Object:
                    case cJSON_NULL:
                    default:
                        LOG_WARN("{0} JSON type {1} not supported here!", __FUNCTION__, (int)jsonStructArr[j]->type);
                        break;
                    }

                    if (shouldBreakForLoop)
                        break;
                }
            }
        }

        template <class T>
        void DeserializeVectorPointer(const cJSON* jsonObj, std::vector<T>** obj) // #TODO Handle vector of primitives (no class type info)
        {
            std::vector<T*>* vectorPtrPtr = (std::vector<T*>*)obj;
            const std::vector<cJSON*> jsonObjectVector = GetAllItemsFromArray(jsonObj);
            if (vectorPtrPtr) // #TODO Validate null pointer value
            {
                vectorPtrPtr->reserve(jsonObjectVector.size());
                vectorPtrPtr->resize(0);
                for (size_t i = 0; i < jsonObjectVector.size(); i++)
                {
                    // #TODO Swap to new method
                    // DeserializeJsonToPointer(jsonObjectVector[i], (T**)obj);

                    T* gameObject = new T();
                    DeserializeJsonArrayToObject(jsonObjectVector[i], Mirror::InfoForClass<T>(), gameObject);
                    vectorPtrPtr->push_back(gameObject);
                }
            }
        }

        template <class T>
        void DeserializeVectorNonPointer(const cJSON* jsonObj, std::vector<T>* obj)
        {
            std::vector<T>* vectorPtr = (std::vector<T>*)obj;
            const std::vector<cJSON*> jsonObjectVector = GetAllItemsFromArray(jsonObj);
            if (vectorPtr) // #TODO Validate null pointer value
            {
                vectorPtr->reserve(jsonObjectVector.size());
                vectorPtr->resize(0);
                for (size_t i = 0; i < jsonObjectVector.size(); i++)
                {
                    T renderable;
                    DeserializeJsonArrayToObject(jsonObjectVector[i], Mirror::InfoForClass<T>(), &renderable);
                    vectorPtr->push_back(renderable);
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

            // This method calls DeserializeJsonArrayToObject but with the type explicitly expressed
            switch (field.type->enumType)
            {
            case MirrorTypes::EngineSettings: // DESERIALIZE_CASE_FOR_CLASS(EngineSettings)
                DeserializeJsonArrayToObject(jsonObj, Mirror::InfoForClass<EngineSettings>(), obj);
                break;

                DESERIALIZE_CASE_FOR_CLASS(ProjectSettings)
                DESERIALIZE_CASE_FOR_CLASS(SceneViewerData)
                DESERIALIZE_CASE_FOR_CLASS(Transform)
                DESERIALIZE_CASE_FOR_CLASS(UserSettings)
                DESERIALIZE_CASE_FOR_CLASS(Vector3)

            case MirrorTypes::m_vector_string:
                {
                    std::vector<std::string>* strings = (std::vector<std::string>*)((char*)obj);
                    const std::vector<cJSON*> jsonStrings = GetAllItemsFromArray(jsonObj);
                    if (strings)
                    {
                        strings->reserve(jsonStrings.size());
                        for (size_t i = 0; i < jsonStrings.size(); i++)
                        {
                            // #TODO Handle more types agnosticly, so this logic would also support ints and other primitives/base values
                            // #TODO This could be templated using the MirrorTypes enum if the function handles more than strings
                            // DeserializeJsonString(jsonObj, MirrorTypes::m_string, obj);

                            strings->push_back(jsonStrings[i]->valuestring);
                        }
                    }
                    else
                    {
                        LOG_WARN("{0} Unable to deserialize type of {1} {2}({3})", __FUNCTION__, field.name, field.type->stringName, (int)field.type->enumType);
                    }
                }
                break;

            case MirrorTypes::m_vector_gameobjectPtr:
                DeserializeVectorPointer(jsonObj, (std::vector<GameObject>**)obj);
                break;

            case MirrorTypes::m_vector_renderable:
                DeserializeVectorNonPointer(jsonObj, (std::vector<Renderable>*)obj);
                break;

            case MirrorTypes::m_vector_routinePtr:
            {
                std::vector<Routine*>* routines = (std::vector<Routine*>*)obj;
                const std::vector<cJSON*> jsonGameObjects = GetAllItemsFromArray(jsonObj);
                if (routines) // #TODO Validate null pointer value
                {
                    routines->reserve(jsonGameObjects.size());
                    routines->resize(0);
                    for (size_t i = 0; i < jsonGameObjects.size(); i++)
                    {
                        // #TODO Handle sub class instantiation

                        eRoutineTypes type = (eRoutineTypes)std::stoi(jsonGameObjects[i]->string);

                        switch (type)
                        {
                        case eRoutineTypes::Routine_Render:
                            DeserializeJsonToPointer(jsonGameObjects[i], (RenderRoutine**)obj);
                        case eRoutineTypes::Routine_Print:
                            // DeserializeJsonToPointer(jsonGameObjects[i], (PrintRoutine**)obj);
                            break;
                        case eRoutineTypes::Routine_Transform:
                            DeserializeJsonToPointer(jsonGameObjects[i], (TransformRoutine**)obj);
                            break;
                        }

                        Routine* routine = *(Routine**)obj;
                        if (routine)
                        {
                            routines->push_back(routine);
                        }
                    }
                }
            }
            break;

            case MirrorTypes::m_map_eComponentTags_componentPtr:
            {
                std::map<eComponentTags, Component*>* componentsMap = (std::map<eComponentTags, Component*>*)obj;
                const std::vector<cJSON*> jsonGameObjects = GetAllItemsFromArray(jsonObj);
                if (componentsMap)
                {
                    componentsMap->clear();
                    for (size_t i = 0; i < jsonGameObjects.size(); i++)
                    {
                        // Component* component = AddComponentToGameObject(jsonGameObjects[i]);

                        switch ((eComponentTags)std::stoi(jsonGameObjects[i]->string))
                        {
                        case Component_Camera:
                            {
                                eCamType camType = eCamType::CamType_FreeCam; // #TODO Get dynamically
                                switch (camType)
                                {
                                case CamType_FreeCam:
                                    DeserializeJsonToPointer(jsonGameObjects[i], (FreeCameraComponent**)obj);
                                    break;

                                case CamType_FirstPerson:
                                    DeserializeJsonToPointer(jsonGameObjects[i], (FirstPersonCameraComponent**)obj);
                                    break;

                                case CamType_ThirdPerson:
                                    DeserializeJsonToPointer(jsonGameObjects[i], (ThirdPersonCameraComponent**)obj);
                                    break;

                                case CamType_Static:
                                    DeserializeJsonToPointer(jsonGameObjects[i], (StaticCameraComponent**)obj);
                                    break;

                                default:
                                    LOG_WARN("{0} TODO", __FUNCTION__);
                                    break;
                                }
                                ComponentCamera* t_pCamComp = *(ComponentCamera**)obj;
                                t_pCamComp->SetType(camType); // #TODO Set in component constructor.
                            }
                            break;

                        case Component_Light:
                            DeserializeJsonToPointer(jsonGameObjects[i], (LightComponent**)obj);
                            (*(LightComponent**)obj)->SetColour(vec3(1.0f, 1.0f, 1.0f));
                            break;

                        case Component_Render:
                            DeserializeJsonToPointer(jsonGameObjects[i], (RenderComponent**)obj);
                            {
                                int bp = 0;
                                // const cJSON* renderables = GetItemFromArrayByKey(jsonGameObjects[i], "m_RenderableList");
                                //
                                // for (size_t i = 0; i < GetArraySize(renderables); i++)
                                // {
                                //     cJSON* renderableArray = GetItemFromArrayByIndex(renderables, i); // renderables->child->child;
                                //
                                //     Renderable renderable;
                                //     renderable.SetRenderableName(renderableArray->string);
                                //
                                //     cJSON* shader = GetItemFromArrayByKey(renderableArray, "Shader");
                                //     cJSON* material = GetItemFromArrayByKey(renderableArray, "Material");
                                //     cJSON* meshFile = GetItemFromArrayByKey(renderableArray, "MeshFile");
                                //     cJSON* meshName = GetItemFromArrayByKey(renderableArray, "MeshName");
                                //
                                //     renderable.SetShader(Resources::GetShaderProgram(shader->valuestring));
                                //     renderable.SetMaterial(Resources::GetMaterial(material->valuestring));
                                //
                                //     // Load Mesh
                                //     if (strcmp(meshFile->valuestring, no_file) == 0)
                                //         renderable.SetMesh(Resources::GetMesh(meshName->valuestring));
                                //     else
                                //         renderable.SetMesh(Resources::GetMeshFromFile(meshFile->valuestring, meshName->valuestring));
                                //
                                //     RenderComponent* renderComponent = *(RenderComponent**)obj;
                                //     renderComponent->AddRenderable(renderable);
                                // }
                            }
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

                        Component* component = *(Component**)obj;
                        if (component)
                        {
                            eComponentTags componentTag = (eComponentTags)std::stoi(jsonGameObjects[i]->string);
                            std::map<eComponentTags, Component*>& componentMapRef = *componentsMap;
                            componentMapRef[componentTag] = component;
                        }
                    }
                }
            }
            break;

            default:
                LOG_WARN("{0} User defined type {1} {2}({3}) not supported here!", __FUNCTION__, field.name, field.type->stringName, (int)field.type->enumType);
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
                    void* sourceAddress = (void*)&jsonObj->valueint;
                    size_t bytesToWrite = sizeof(jsonObj->valueint);

                    switch (type)
                    {
                    case MirrorTypes::m_float:
                    {
                        float* a = (float*)fieldAddress;
                        *a = (float)jsonObj->valuedouble;
                        break;
                    }

                    case MirrorTypes::m_double:
                        sourceAddress = (void*)&jsonObj->valuedouble;
                        bytesToWrite = sizeof(double);
                        // Fall through to default case
                    default:
                        memcpy(fieldAddress, sourceAddress, bytesToWrite);
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

        // #Unused
        template <class T>
        void DeserializeJsonToCollection(const cJSON* jsonObj, const MirrorTypes type, void* obj)
        {
            // Handle collection specific logic like iterating over the JSON list, calling
            // creation method recursively.

            std::vector<T>* objectVectorPtr = (std::vector<T>*)((char*)obj);
            const std::vector<cJSON*> jsonObjectVector = GetAllItemsFromArray(jsonObj);
            if (objectVectorPtr) // #TODO Validate null pointer value
            {
                objectVectorPtr->reserve(jsonObjectVector.size());
                for (size_t i = 0; i < jsonObjectVector.size(); i++)
                {
                    // T routine = AddRoutineToGameObject(jsonObjectVector[i]);
                    // if (routine)
                    // {
                    //     objectVectorPtr->push_back(routine);
                    // }
                }
            }
        }

        // #Unused
        void _DeserializeJsonToPointer(const cJSON* jsonObj, const MirrorTypes type, void* obj)
        {
            // #TODO Define the proper way to handle memory deallocation
            // #TODO Look at templating the type to be type-agnostic

            // #NOTE This method instantiates the type as a new object
            switch (type)
            {
            case MirrorTypes::m_charPtr:
            case MirrorTypes::m_constCharPtr:
                {
                    char** fieldAddress = (char**)((char*)obj);
                    *fieldAddress = (char*)_strdup(jsonObj->valuestring);
                }
                break;

            default:
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

        void SerializeField(cJSON* objJson, const Mirror::Field& field, const void* obj);

#define SERIALIZE_CASE_FOR_CLASS(ClassType) \
case MirrorTypes::ClassType: \
    SerializeJsonObject(arr, Mirror::InfoForClass<ClassType>(), (char*)obj + field.offset); \
    break; \

        void SerializeJsonObject(cJSON* objJson, const Mirror::ClassInfo* objClassInfo, const void* obj)
        {
            if (!objJson || !objClassInfo || !obj)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            for (size_t i = 0; i < objClassInfo->fields.size(); i++)
            {
                const Mirror::Field& field = objClassInfo->fields[i];

                if (!field.type)
                    break;

                if (field.type->enumType < MirrorTypes::m_PRIMITIVES_START)
                {
                    cJSON* arr = CreateArray(field.name.c_str());
                    cJSON_AddItemToArray(objJson->child, arr);

                    switch (field.type->enumType)
                    {
                    case MirrorTypes::EngineSettings: // SERIALIZE_CASE_FOR_CLASS(EngineSettings)
                        SerializeJsonObject(arr, Mirror::InfoForClass<EngineSettings>(), (char*)obj + field.offset);
                        break;

                        SERIALIZE_CASE_FOR_CLASS(ProjectSettings)
                        SERIALIZE_CASE_FOR_CLASS(SceneViewerData)
                        SERIALIZE_CASE_FOR_CLASS(Transform)
                        SERIALIZE_CASE_FOR_CLASS(UserSettings)
                        SERIALIZE_CASE_FOR_CLASS(Vector3)

                    case MirrorTypes::m_vector_string:
                        {
                            std::vector<std::string>* strings = (std::vector<std::string>*)((char*)obj + field.offset);
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
                                LOG_WARN("{0} Unable to deserialize type of {1} {2}({3})", __FUNCTION__, field.name, field.type->stringName, (int)field.type->enumType);
                            }
                        }
                        break;

                    case MirrorTypes::m_vector_gameobjectPtr:
                        {
                            std::vector<GameObject*>* gameObjects = (std::vector<GameObject*>*)((char*)obj + field.offset);
                            if (gameObjects)
                            {
                                for (size_t i = 0; i < gameObjects->size(); i++)
                                {
                                    // AddItemToArray(arr, DataManager::ConvertGameObjectToJSON(gameObjects->at(i)));
                                    AddItemToArray(arr, ConvertGameObjectToJSON(gameObjects->at(i)));
                                }
                            }
                        }
                        break;

                    default:
                        LOG_WARN("{0} Unsupported user defined field type {1} {2}({3}) for serialization!", __FUNCTION__, field.name, field.type->stringName, (int)field.type->enumType);
                        break;
                    }
                }
                else
                {
                    SerializeField(objJson, field, obj); // #TODO Replace with field agnostic function
                }
            }
        }

        void SerializeField(cJSON* objJson, const Mirror::Field& field, const void* obj)
        {
            if (!objJson || !obj)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (!field.type || field.type->enumType < MirrorTypes::m_PRIMITIVES_START)
            {
                LOG_ERROR("{0} Invalid field passed!", __FUNCTION__);
                return;
            }

            switch (field.type->enumType)
            {
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

            case MirrorTypes::m_char:
            case MirrorTypes::eKeys:
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

            case MirrorTypes::m_int8_t:
            case MirrorTypes::m_int16_t:
            case MirrorTypes::m_int32_t:
            case MirrorTypes::m_int64_t:
            case MirrorTypes::m_uint8_t:
            case MirrorTypes::m_uint16_t:
            case MirrorTypes::m_uint32_t:
            case MirrorTypes::m_uint64_t:
            case MirrorTypes::m_int:
            case MirrorTypes::m_float:
            case MirrorTypes::m_double:
            case MirrorTypes::m_eSceneTypes:
                {
                    int* numberAddress = (int*)((char*)obj + field.offset);
                    AddItemToArray(objJson, CreateNumber(field.name.c_str(), *numberAddress));
                }
                break;

            default:
                LOG_ERROR("{0} Unsupported field type {1} {2}({3}) for serialization!", __FUNCTION__, field.name, field.type->stringName, (int)field.type->enumType);
                break;
            }
        }

    }

}