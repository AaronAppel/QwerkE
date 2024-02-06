#include "QF_Scenes.h"

#include <map>

#include "QF_Defines.h"
#include "QF_Enums.h"
#include "QF_FileUtilities.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Profile.h"
#include "QF_Scene.h"
#include "QF_Settings.h"

namespace QwerkE {

    bool Scenes::m_IsRunning = true;
    Scene* Scenes::m_CurrentScene = nullptr;
    std::map<eSceneTypes, Scene*> Scenes::m_Scenes; // #TODO Review using a vector instead. Lookup can't be that costly

	Scenes::~Scenes()
	{
		for (auto it = m_Scenes.begin(); it != m_Scenes.end(); it++)
		{
			delete it->second;
		}
	}

	void Scenes::Initialize()
	{
		const ProjectSettings& projectSettings = Settings::GetProjectSettings();

		// #TODO How should the file names be given to the scene?
		const std::vector<std::string>& sceneFileNames = projectSettings.sceneFileNames;

		for (size_t i = 0; i < sceneFileNames.size(); i++)
		{
			const char* sceneFileName = sceneFileNames[0].c_str();
			if (FileExists(ScenesFolderPath(sceneFileName)) == false)
			{
				LOG_WARN("Scenes::Initialize(): File not found: {0}", sceneFileName);
				continue;
			}

			Scene* newScene = new Scene(sceneFileName);
			newScene->LoadScene();
			AddScene(newScene, i == 0); // #TODO Improve default starting scene selection/specification
		}

		if (m_Scenes.empty())
        {
			Scene* emptyScene = new Scene(null_scene);
			emptyScene->Initialize();
			emptyScene->LoadScene();
			emptyScene->SetIsEnabled(true);
			AddScene(emptyScene, true);
			LOG_WARN("Null scene loaded as no scene files found for project: {0}", projectSettings.projectName);
		}
	}

	void Scenes::EnableScene(eSceneTypes type)
	{
		if (m_Scenes.find(type) != m_Scenes.end())
		{
			m_Scenes[type]->SetIsEnabled(true);
		}
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
		if (m_Scenes.find(type) != m_Scenes.end())
		{
			m_Scenes[type]->SetIsEnabled(false);
		}
	}

	void Scenes::DisableAll()
	{
		for (auto it = m_Scenes.begin(); it != m_Scenes.end(); it++)
		{
			it->second->SetIsEnabled(false);
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
		return nullptr;
	}

	void Scenes::Update(float deltatime)
	{
		PROFILE_SCOPE("Scene Manager Update");

		if (Input::FrameKeyAction(eKeys::eKeys_P, eKeyState::eKeyState_Press))
		{
			GetCurrentScene()->ToggleIsPaused();
		}

		if (m_CurrentScene && m_IsRunning && m_CurrentScene->GetIsEnabled())
		{
			m_CurrentScene->Update(deltatime);
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
		if (m_Scenes.find(scene) != m_Scenes.end())
		{
			m_Scenes[scene]->Draw();
		}
	}

}
