#include "QF_ViewerScene.h"

#include "QF_Resources.h"
#include "QF_Factory.h"

// #TODO Move this code and file out of the framework, and into the engine.
// Should probably be made into a data file and loaded by the engine

namespace QwerkE {

    ViewerScene::ViewerScene()
    {
        m_ID = eSceneTypes::Scene_ViewerScene;
        m_LevelFileName = "ViewerScene.qscene";
    }

    void ViewerScene::Initialize()
    {
        Factory::CreateFreeCamera(this, vec3(0, 0, 5));
        Scene::SetupCameras();

        Factory::CreateLight(this, vec3(0, 5, -10));
    }

}
