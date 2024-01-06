#include "GameScene.h"
#include "../../../QwerkE_Framework/Source/Core/Factory/Factory.h"
#include "../../../QwerkE_Framework/Source/Core/Resources/Resources.h"
#include "../../../QwerkE_Framework/Source/Core/Time/Time.h"
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
    Scene::SetupCameras();

    {
        // Create scene cameras
        Factory::CreateFreeCamera(this, vec3(0, 0, 5));
        // setup view/projection matrices
        Scene::SetupCameras();
    }

    {	// Create scene objects
        // cubes
        Factory::CreatePlane(this, vec3(0, -1, 40));

        obj2 = Factory::CreateCube(this, vec3(0, 1, 40));
        obj2->SetRotation(vec3(45, 45, 45));
        obj2->AddComponent((Component*)new PrintComponent());
        obj2->AddRoutine((Routine*)new PrintRoutine());
    }

    {	// Create scene lighting
        int lights = 0;
        for (int i = 0; i < lights; i++)
        {
            Factory::CreateLight(this, vec3((i + 1) * -3.0f, (i + 1) * -1.0f, (i + 1) * -3.0f));
        }
        Factory::CreateLight(this, vec3(0, 5, -10));
    }
}

void GameScene::p_Update(double deltatime)
{
	obj2->SetRotation(vec3(obj2->GetRotation().x,
		obj2->GetRotation().y + 0.002f / QwerkE::Time::GetDeltaTime(),
		0));
    Scene::p_Update(deltatime);
}

void GameScene::Draw()
{
    Scene::Draw(m_CameraList.At(m_CurrentCamera));
}