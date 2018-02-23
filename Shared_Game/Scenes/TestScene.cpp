#include "stdafx.h"

TestScene::TestScene(GameCore* pGame) :
	Scene(pGame)
{
	m_pSceneManager = pGame->GetSceneManager();
	m_ID = Scene_TestScene;
	m_LevelFileDir = "Resources/LevelData/TestScene.json";
}

TestScene::~TestScene()
{
	m_CameraList.Clear();
}

void TestScene::Initialize()
{
	Factory* t_pFactory = m_pGameCore->GetFactory();
	ResourceManager* t_pResourceManager = m_pGameCore->GetResourceManager();
	DataManager* t_pDataManager = m_pGameCore->GetDataManager();

	t_pDataManager->LoadScene(this, m_LevelFileDir); // Load scene
	Scene::SetupCameras();

	if (false)
	{
		{
			// Create scene cameras
			const int t_CamMax = 5;
			for (int i = 0; i < t_CamMax; i++)
			{
				t_pFactory->CreateFreeCamera(this, vec3(0, 0, -2.5f * (i + 2) * 2));
			}
			// setup view/projection matrices
			Scene::SetupCameras();
		}

		{	// Create scene objects
			// cubes
			int cubes = 0;
			for (float i = 0; i < cubes; i++)
			{
				t_pFactory->CreateCube(this, vec3(i, i, i));
			}
			GameObject* nanosuit = t_pFactory->CreateTestModel(this, vec3(0, -1, 0));
			if (nanosuit)
			{
				nanosuit->SetRotation(vec3(0, 180, 0));
			}

			// plane
			t_pFactory->CreatePlane(this, vec3(0, -1, 0));
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
}

void TestScene::p_Update(double deltatime)
{
	Scene::p_Update(deltatime);
}

void TestScene::Draw()
{
	Scene::Draw(m_CameraList.At(m_CurrentCamera));
}