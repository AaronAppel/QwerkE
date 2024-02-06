#include "QF_Scenes.h"

#include "QF_TestScene.h"
#include "QF_GameObject.h"
#include "QF_Enums.h"
#include "QF_Factory.h"
#include "QF_Mesh.h"
#include "QF_MeshFactory.h"
#include "QF_Resources.h"
#include "QF_Serialization.h"

namespace QwerkE {

	TestScene::TestScene() : Scene("TestScene.qscene")
	{
		m_ID = eSceneTypes::Scene_TestScene;
	}

	void TestScene::Initialize()
	{
		GameObject* newCamera = new GameObject(this);
		Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Camera.osch"), *newCamera);
		newCamera->SetPosition(vec3(0, 0, 5));
		newCamera->OnSceneLoaded(this);
		this->AddCamera(newCamera);
		Scene::SetupCameras();

		GameObject* newGameObject = new GameObject(this);
		Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Nanosuit.osch"), *newGameObject);
		newGameObject->SetPosition(vec3(0, -2, 30));
		newGameObject->OnSceneLoaded(this);
		this->AddCamera(newGameObject);

		Factory::CreateTestCube(this, vec3(0, 0, 50));

		GameObject* newLight = new GameObject(this);
		Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Light.osch"), *newLight);
		newLight->SetPosition(vec3(0, 5, -10));
		newLight->OnSceneLoaded(this);
		this->AddLight(newLight);
	}

}
