#include "GameScene.h"
#include "../Shared_Engine/Systems/ServiceLocator.h"
#include "../Shared_Engine/Systems/Factory/Factory.h"
#include "../Shared_Engine/Systems/ResourceManager.h"
#include "../Shared_Engine/Systems/Time.h"
#include "PrintComponent.h"
#include "PrintRoutine.h"

GameScene::GameScene()
{
    m_ID = Scene_GameScene;
    Initialize();
}

GameScene::~GameScene()
{
    m_CameraList.Clear();
}

GameObject* obj2;
void GameScene::Initialize()
{
    Factory* t_pFactory = (Factory*)QwerkE::ServiceLocator::GetService(eEngineServices::Factory_Entity);
    ResourceManager* t_pResourceManager = (ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager);
    // TODO: Resolve feature
    //DataManager* t_pDataManager = m_pGameCore->GetDataManager();

    //t_pDataManager->LoadScene(this, m_LevelFileDir); // Load scene
    Scene::SetupCameras();

    // return;
    {
        // Create scene cameras
        const int t_CamMax = 1;
        t_pFactory->CreateFreeCamera(this, vec3(0, 0, 5));
        // setup view/projection matrices
        Scene::SetupCameras();
    }

    {	// Create scene objects
        // cubes
        int cubes = 1;
        obj2 = t_pFactory->CreateCube(this, vec3(0, -5, 60));
        obj2->SetRotation(vec3(45, 45, 45));
        obj2->AddComponent((Component*)new PrintComponent());
        obj2->AddRoutine((Routine*)new PrintRoutine());
    }

    {	// Create scene lighting
        int lights = 0;
        for (int i = 0; i < lights; i++)
        {
            t_pFactory->CreateLight(this, vec3((i + 1) * -3.0f, (i + 1) * -1.0f, (i + 1) * -3.0f));
        }
        t_pFactory->CreateLight(this, vec3(0, 5, -10));
    }
}

void GameScene::p_Update(double deltatime)
{
    Scene::p_Update(deltatime);
}

void GameScene::Draw()
{
    Scene::Draw(m_CameraList.At(m_CurrentCamera));
}