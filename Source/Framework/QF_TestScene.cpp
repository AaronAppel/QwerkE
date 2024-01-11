#include "QF_Scenes.h"

#include "QF_TestScene.h"
#include "QF_DataManager.h"
#include "QF_GameObject.h"
#include "QF_Enums.h"
#include "QF_Factory.h"
#include "QF_Mesh.h"
#include "QF_MeshFactory.h"
#include "QF_Resources.h"

namespace QwerkE {

	TestScene::TestScene() : Scene("TestScene.qscene")
	{
		m_ID = eSceneTypes::Scene_TestScene;
	}

	GameObject* obj; // #TODO Fix hack
	void TestScene::Initialize()
	{
		// TODO: Resolve feature
		//DataManager* t_pDataManager = m_pGameCore->GetDataManager();

		//t_pDataManager->LoadScene(this, m_LevelFileDir); // Load scene

		// return;
		{
			// Create scene cameras
			const int t_CamMax = 1;
			Factory::CreateFreeCamera(this, vec3(0, 0, 5));
			// Factory::CreateFreeCamera(this, vec3(0, 0, 15));
			// setup view/projection matrices
			Scene::SetupCameras();
		}

		{	// Create scene objects
			// cubes
			int cubes = 2;
			Factory::CreateTestCube(this, vec3(0, 0, 50));
			// Factory::CreateCube(this, vec3(2, 2, 30));
			// Factory::CreateCube(this, vec3(-2, 1, 40));

			// TEST:
			// obj = Factory::CreateTestModel(this, vec3(0, -2, 30));
			// obj->SetRotation(vec3(0, 0, 0));

			for (int i = 0; i < 10; i++)
			{
				// Factory::CreateCube(this, vec3(0, i - 5, 50));
			}
			// plane
			// Factory::CreatePlane(this, vec3(0, -1, 0));
		}

		{	// Create scene lighting
			int lights = 3;
			for (int i = 0; i < lights; i++)
			{
				// Factory::CreateLight(this, vec3((i + 1) * -3.0f, (i + 1) * -1.0f, (i + 1) * -3.0f));
			}
			Factory::CreateLight(this, vec3(0, 5, -10));
		}
	}

	void TestScene::Update(float deltatime)
	{
		/*obj->SetRotation(vec3(obj->GetRotation().x,
			obj->GetRotation().y + 90.0f * deltatime,
			obj->GetRotation().z));

		if (obj->GetRotation().y >= 360.0f)
			obj->SetRotation(vec3(obj->GetRotation().x, obj->GetRotation().y - 360.0f, obj->GetRotation().z));*/
	}

}
