#include "QF_Serialization.h"

#include "Libraries/cJSON/QC_cJSON.h"

#include "QF_DataManager.h"
#include "QF_GameObject.h"
#include "QF_Log.h"
#include "QF_Renderable.h"
#include "QF_Resources.h"
#include "QF_ShaderComponent.h"

#include "QF_FileUtilities.h"
#include "QF_Material.h"

#include "QF_Component.h"
#include "QF_RenderComponent.h"
#include "QF_LightComponent.h"

#include "QF_CameraComponent.h"
#include "QF_FirstPersonCameraComponent.h"
#include "QF_FreeCameraComponent.h"
#include "QF_StaticCameraComponent.h"
#include "QF_ThirdPersonCameraComponent.h"

#include "QF_Bullet3Routine.h" // Routines
#include "QF_RenderRoutine.h"
#include "QF_Routine.h"
#include "QF_TransformRoutine.h"

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

    }

}