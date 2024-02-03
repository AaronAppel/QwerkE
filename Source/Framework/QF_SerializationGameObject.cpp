#include "QF_Serialization.h"

#include "Libraries/cJSON/QC_cJSON.h"

#include "QF_DataManager.h"
#include "QF_GameObject.h"
#include "QF_Log.h"
#include "QF_Renderable.h"
#include "QF_Resources.h"
#include "QF_ShaderComponent.h"

#include "QF_CameraComponent.h" // Components
#include "QF_Component.h"
#include "QF_FileUtilities.h"
#include "QF_FirstPersonCameraComponent.h"
#include "QF_FreeCameraComponent.h"
#include "QF_LightComponent.h"
#include "QF_Material.h"
#include "QF_RenderComponent.h"
#include "QF_StaticCameraComponent.h"
#include "QF_ThirdPersonCameraComponent.h"

#include "QF_Bullet3Routine.h" // Routines
#include "QF_RenderRoutine.h"
#include "QF_Routine.h"
#include "QF_TransformRoutine.h"

// #TODO Deprecate
#define no_file "None" // #TODO Consider coupling to cJSON logic and moving out of here

// #TODO Look at removing from QwerkE namespace, or have a #using QwerkE just in this .cpp.
// Helps with porting to a stand alone library, and encourages type indifference/independence
namespace QwerkE {

    namespace Serialization {

        cJSON* ConvertGameObjectToJSON(void* obj)
        {
            GameObject* object = (GameObject*)obj;
            if (object == nullptr)
                return nullptr;

            cJSON* t_ReturnJSON = CreateArray(object->GetName().c_str());

            GameObject& objectRef = *object;
            // SerializeObject(t_ReturnJSON, objectRef);

            const vec3& position = object->GetPosition();
            SerializeObject(t_ReturnJSON, position);

            cJSON* transform = CreateArray(Mirror::InfoForType<Transform>()->stringName.c_str());
            DataManager::AddPositionTocJSONItem(transform, object);
            AddItemToArray(t_ReturnJSON, transform);

            cJSON* t_Position = CreateArray("Vector3");
            AddItemToArray(t_Position, CreateNumber((char*)"x", object->GetPosition().x));
            AddItemToArray(t_Position, CreateNumber((char*)"PositionY", object->GetPosition().y));
            AddItemToArray(t_Position, CreateNumber((char*)"PositionZ", object->GetPosition().z));
            AddItemToArray(t_ReturnJSON, t_Position);

            DataManager::AddRotationTocJSONItem(t_ReturnJSON, object);
            DataManager::AddScaleTocJSONItem(t_ReturnJSON, object);

            AddItemToArray(t_ReturnJSON, CreateNumber((char*)"ObjectTag", (int)object->GetTag()));
            DataManager::AddComponentsTocJSONItem(t_ReturnJSON, object);
            DataManager::AddRoutinesTocJSONItem(t_ReturnJSON, object);

            // #TODO Handle special cases for certain components
            // switch (object->GetTag())
            // {
            // default:
            //     break;
            // }

            return t_ReturnJSON;
        }

        GameObject* ConvertJSONToGameObject(cJSON* item)
        {
            GameObject* object = new GameObject();
            GameObject& objectRef = *object;

            DeserializeJsonArrayToObject(item, Mirror::InfoForClass<GameObject>(), (void*)&objectRef);

            return object;
        }

        Component* AddComponentToGameObject(cJSON* item)
        {
            Component* returnComponent = nullptr;
            if (!item) { return returnComponent; }

            switch ((eComponentTags)std::stoi(item->string))
            {
            case Component_Camera:
            {
                ComponentCamera* t_pCamComp = nullptr;
                eCamType camType = eCamType::CamType_FreeCam; // #TODO Get dynamically
                switch (camType)
                {
                case CamType_FreeCam:
                    t_pCamComp = new FreeCameraComponent();
                    break;
                case CamType_FirstPerson:
                    t_pCamComp = new FirstPersonCameraComponent();
                    break;
                case CamType_ThirdPerson:
                    t_pCamComp = new ThirdPersonCameraComponent();
                    break;
                case CamType_Static:
                    t_pCamComp = new StaticCameraComponent();
                    break;
                }
                t_pCamComp->SetType(camType); // #TODO Set in component constructor.
                returnComponent = t_pCamComp;
            }
            break;

            case Component_Light:
            {
                LightComponent* t_pLightComp = new LightComponent();
                t_pLightComp->SetColour(vec3(1.0f, 1.0f, 1.0f));
                returnComponent = t_pLightComp;
            }
            break;

            case Component_Render:
            {
                RenderComponent* rComp = new RenderComponent();
                const cJSON* renderables = GetItemFromArrayByKey(item, "Renderables");

                for (size_t i = 0; i < GetArraySize(renderables); i++)
                {
                    cJSON* renderableArray = GetItemFromArrayByIndex(renderables, i); // renderables->child->child;

                    Renderable renderable;
                    renderable.SetRenderableName(renderableArray->string);

                    cJSON* shader = GetItemFromArrayByKey(renderableArray, "Shader");
                    cJSON* material = GetItemFromArrayByKey(renderableArray, "Material");
                    cJSON* meshFile = GetItemFromArrayByKey(renderableArray, "MeshFile");
                    cJSON* meshName = GetItemFromArrayByKey(renderableArray, "MeshName");

                    renderable.SetShader(Resources::GetShaderProgram(shader->valuestring));
                    renderable.SetMaterial(Resources::GetMaterial(material->valuestring));

                    // Load Mesh
                    if (strcmp(meshFile->valuestring, no_file) == 0)
                        renderable.SetMesh(Resources::GetMesh(meshName->valuestring));
                    else
                        renderable.SetMesh(Resources::GetMeshFromFile(meshFile->valuestring, meshName->valuestring));

                    rComp->AddRenderable(renderable);
                }
                returnComponent = rComp;
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
                // #TODO Implement component adds
                break;
            }

            return returnComponent;
        }

        Routine* AddRoutineToGameObject(cJSON* item)
        {
            Routine* routine = nullptr;
            if (!item) { return routine; }

            eRoutineTypes type = (eRoutineTypes)std::stoi(item->string);

            switch (type)
            {
            case eRoutineTypes::Routine_Render:
                routine = new RenderRoutine();
            case eRoutineTypes::Routine_Print:
                // #TODO Debug
                // object->AddDrawRoutine((Routine*)new PrintRoutine());
                break;
            case eRoutineTypes::Routine_Transform:
                cJSON* transformValues = GetItemFromArrayByKey(item, "TransformValues");
                cJSON* pos = GetItemFromArrayByKey(transformValues, "Position");
                vec3 position = vec3((float)GetItemFromArrayByIndex(pos, 0)->valuedouble,
                    (float)GetItemFromArrayByIndex(pos, 1)->valuedouble,
                    (float)GetItemFromArrayByIndex(pos, 2)->valuedouble);
                cJSON* rot = GetItemFromArrayByKey(transformValues, "Rotation");
                vec3 rotation = vec3((float)GetItemFromArrayByIndex(rot, 0)->valuedouble,
                    (float)GetItemFromArrayByIndex(rot, 1)->valuedouble,
                    (float)GetItemFromArrayByIndex(rot, 2)->valuedouble);
                cJSON* sca = GetItemFromArrayByKey(transformValues, "Scale");
                vec3 scale = vec3((float)GetItemFromArrayByIndex(sca, 0)->valuedouble,
                    (float)GetItemFromArrayByIndex(sca, 1)->valuedouble,
                    (float)GetItemFromArrayByIndex(sca, 2)->valuedouble);

                TransformRoutine* transform = new TransformRoutine();
                transform->SetSpeed((float)GetItemFromArrayByKey(item, "Speed")->valuedouble);
                transform->SetPositionOff(position);
                transform->SetRotationOff(rotation);
                transform->SetScaleOff(scale);
                routine = transform;
                break;
            }

            return routine;
        }

    }

}