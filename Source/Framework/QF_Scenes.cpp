#include "QF_Scenes.h"

#include <map>

#include "Libraries/cJSON/QC_cJSON.h"

#include "QF_Defines.h"
#include "QF_Log.h"
#include "QF_Scene.h"
#include "QF_TestScene.h"
#include "QF_ViewerScene.h"
#include "QF_PBR_Test1.h"
#include "QF_Enums.h"
#include "QF_FileUtilities.h"
#include "QF_Profile.h"
#include "QF_Input.h"

namespace QwerkE {

    bool Scenes::m_IsRunning = true;
    Scene* Scenes::m_CurrentScene = nullptr;
    std::map<eSceneTypes, Scene*> Scenes::m_Scenes;

	Scenes::Scenes()
	{
		//m_pController = new PlayerController();
	}

	Scenes::~Scenes()
	{
		for (unsigned int i = 0; i < m_Scenes.size(); i++)
		{
			delete m_Scenes[(eSceneTypes)i];
		}
	}

	void Scenes::Initialize()
	{
        // TODO: Scenes should be added somewhere else
        // TODO: Load scenes from the Resources/Scenes/... folder to use persistent data
	 	// auto scenes = Resources::SeeScenes();
		// if (scenes.empty())
		// {
		//      Create empty scene
		// }

		const char* prefPath = ConfigsFolderPath(null_preferences);
		// TODO: does this free memory correctly? std::string pref = ConfigsFolderPath(null_config);

		if (FileExists(prefPath))
		{
			cJSON* root = OpencJSONStream(prefPath);
			cJSON* scenesArray = GetItemFromRootByKey(root, "Scenes");

			unsigned int numScenes = GetArraySize(scenesArray);
			for (unsigned int i = 0; i < numScenes; i++)
			{
				cJSON* scene = GetItemFromArrayByIndex(scenesArray, i);

				const char* sceneFileName = scene->valuestring;

				if (FileExists(ScenesFolderPath(sceneFileName)) == false)
                {
                    LOG_WARN("Scenes::Initialize(): File not found: {0}", sceneFileName);
					continue;
				}

				Scene* newScene = new Scene(sceneFileName); // TODO: Improve how scene file names are assigned
				newScene->LoadScene();
				AddScene(newScene, i == 0); // #TODO Improve default starting scene selection/specification
			}

			ClosecJSONStream(root);
		}

		if (m_Scenes.empty())
        {
			Scene* emptyScene = new Scene(null_scene);
			emptyScene->Initialize();
			emptyScene->LoadScene();
			emptyScene->SetIsEnabled(true);
			AddScene(emptyScene, true);
            LOG_WARN("Null scene loaded because no valid scene was found in: {0}", prefPath);
		}
		// TODO: Delete or manage prefPath memory
	}

	void Scenes::EnableScene(eSceneTypes type)
	{
		m_Scenes[type]->SetIsEnabled(true);
	}

	void Scenes::SetCurrentScene(eSceneTypes type)
	{
		if (m_Scenes.find(type) != m_Scenes.end())
		{
			m_CurrentScene = m_Scenes[type];
		}
		else
		{
			LOG_ERROR("Scene not in scenes list!");
		}
	}

	void Scenes::DisableScene(eSceneTypes type)
	{
		m_Scenes[type]->SetIsEnabled(false);
	}

	void Scenes::DisableAll()
	{
		for (unsigned int i = 0; i < m_Scenes.size(); i++)
		{
			Scene* temp = m_Scenes[(eSceneTypes)i];
			temp->SetIsEnabled(false);
		}
	}

	void Scenes::AddScene(Scene* scene, bool setAsCurrentScene)
	{
		if (m_Scenes.find(scene->GetSceneID()) == m_Scenes.end())
		{
			m_Scenes[scene->GetSceneID()] = scene;

			if (setAsCurrentScene)
			{
				m_CurrentScene = scene;
			}
		}
	}

	Scene* Scenes::RemoveScene(Scene* scene)
	{
		if (m_Scenes.find(scene->GetSceneID()) != m_Scenes.end())
		{
			m_Scenes.erase(scene->GetSceneID());
			Scene* returnScene = m_Scenes[scene->GetSceneID()];
			return returnScene;
		}
		else
			return nullptr;
	}

	void Scenes::Update(float deltatime)
	{
		PROFILE_SCOPE("Scene Manager Update");

		if (Input::FrameKeyAction(eKeys::eKeys_P, eKeyState::eKeyState_Press))
		{
			GetCurrentScene()->ToggleIsPaused();
		}

		if (m_CurrentScene && m_IsRunning)
		{
			if (m_CurrentScene->GetIsEnabled())
			{
				m_CurrentScene->Update(deltatime);
			}
		}
	}

	void Scenes::DrawCurrentScene()
    {
        PROFILE_SCOPE("Scene Manager Render");
		if (m_CurrentScene && m_CurrentScene->GetIsEnabled())
		{
			m_CurrentScene->Draw();
		}
	}

	void Scenes::DrawScene(eSceneTypes scene)
	{
		m_Scenes[scene]->Draw();
	}

}
