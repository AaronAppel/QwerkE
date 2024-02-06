#include "QF_ViewerScene.h"

#include "QF_Factory.h"
#include "QF_Resources.h"
#include "QF_Serialization.h"

// #TODO Move this code and file out of the framework, and into the engine.
// Should probably be made into a data file and loaded by the engine

namespace QwerkE {

    ViewerScene::ViewerScene() : Scene("ViewerScene.qscene")
    {
        m_ID = eSceneTypes::Scene_ViewerScene;
    }

    void ViewerScene::Initialize()
    {
        GameObject* newCamera = new GameObject(this);
        Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Camera.osch"), *newCamera);
        newCamera->SetPosition(vec3(0, 0, 5));
        newCamera->OnSceneLoaded(this);
        this->AddCamera(newCamera);
        Scene::SetupCameras();

        GameObject* newLight = new GameObject(this);
        Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Light.osch"), *newLight);
        newLight->SetPosition(vec3(0, 0, -10));
        newLight->OnSceneLoaded(this);
        this->AddLight(newLight);
    }

}
