#include "Factory.h"

// #TODO Rename file to something better and factory related

#include <string>

#include "QC_Helpers.h"
#include "QC_StringHelpers.h"

#include "QF_FileSystem.h"

#include "QF_Resources.h"

#include "QF_CameraComponent.h"
#include "QF_FirstPersonCameraComponent.h"
#include "QF_FreeCameraComponent.h"
#include "QF_StaticCameraComponent.h"
#include "QF_ThirdPersonCameraComponent.h"
#include "QF_RenderRoutine.h"
#include "QF_RenderComponent.h"
#include "QF_GameObject.h"
#include "QF_Scene.h"

namespace QwerkE {

    GameObject* Factory::CreateFreeCamera(Scene* scene, vec3 position)
    {
        return InternalCreateCamera(scene, position, CamType_FreeCam); // #TODO Is this a good way of re-using CreateCamera()?
    }

    GameObject* Factory::CreateFirstPersonCamera(Scene* scene, vec3 position)
    {
        return InternalCreateCamera(scene, position, CamType_FirstPerson);
    }

    GameObject* Factory::CreateThirdPersonCamera(Scene* scene, vec3 position)
    {
        return InternalCreateCamera(scene, position, CamType_ThirdPerson);
    }

    GameObject* Factory::CreateStaticCamera(Scene* scene, vec3 position)
    {
        return InternalCreateCamera(scene, position, CamType_Static);
    }

    GameObject* Factory::InternalCreateCamera(Scene* scene, vec3 position, eCamType camType)
    {
        GameObject* t_pCamera = CreateGameObject(scene);
        t_pCamera->SetTag(GO_Tag_Camera);
        t_pCamera->SetPosition(position);
        t_pCamera->SetRenderOrder(-1);
        t_pCamera->SetName("Camera" + std::to_string(helpers_GetUniqueID()));

        CameraComponent* t_pCamComp = nullptr;

        switch (camType)
        {
        case CamType_FreeCam:
            t_pCamComp = new FreeCameraComponent();
            t_pCamComp->SetType(CamType_FreeCam);
            break;
        case CamType_FirstPerson:
            t_pCamComp = new FirstPersonCameraComponent();
            t_pCamComp->SetType(CamType_FirstPerson);
            break;
        case CamType_ThirdPerson:
            t_pCamComp = new ThirdPersonCameraComponent();
            t_pCamComp->SetType(CamType_ThirdPerson);
            break;
        case CamType_Static:
            t_pCamComp = new StaticCameraComponent();
            t_pCamComp->SetType(CamType_Static);
            break;
        }

        t_pCamera->AddComponent(t_pCamComp);
        t_pCamComp->Setup();
        t_pCamComp->SetTargetPosition(vec3(0, 0, 0));

        // #TODO Review line below
        // ((FileSystem*)QwerkE::Services::GetService(eEngineServices::FileSystem))->LoadModelFileToMeshes(MeshesFolderPath("Camera.obj"));
        AddModelComponentFromSchematic(t_pCamera, "camera.osch");

        RenderRoutine* renderRoutine = new RenderRoutine();
        t_pCamera->AddRoutine((Routine*)renderRoutine);

        if (scene->AddCamera(t_pCamera))
        {
            m_CreatedCount++;
            return t_pCamera;
        }

        delete t_pCamera;
        delete t_pCamComp;
        return nullptr;
    }

}
