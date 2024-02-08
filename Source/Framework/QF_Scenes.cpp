#include "QF_Scenes.h"

#include <vector>

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
    std::vector<Scene*> Scenes::m_Scenes;

	void Scenes::Initialize()
	{
		const ProjectSettings& projectSettings = Settings::GetProjectSettings();

		// #TODO How should the file names be given to the scene?
		const std::vector<std::string>& sceneFileNames = projectSettings.sceneFileNames;

		for (size_t i = 0; i < sceneFileNames.size(); i++)
		{
			const char* sceneFileName = sceneFileNames[i].c_str();
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
			emptyScene->LoadScene();
			emptyScene->SetIsEnabled(true);
			AddScene(emptyScene, true);
			LOG_WARN("Null scene loaded as no scene files found for project: {0}", projectSettings.projectName);
		}
	}

	void Scenes::Shutdown()
	{
		for (size_t i = 0; i < m_Scenes.size(); i++)
		{
			if (m_Scenes[i])
			{
				delete m_Scenes[i];
			}
		}
	}

	void Scenes::EnableScene(std::string sceneName)
	{
		if (Scene* scene = GetScene(sceneName))
		{
			scene->SetIsEnabled(true);
		}
	}

	void Scenes::SetCurrentScene(std::string sceneName)
	{
		if (Scene* scene = GetScene(sceneName))
		{
			m_CurrentScene = scene;
		}
	}

	void Scenes::DisableScene(std::string sceneName)
	{
		if (Scene* scene = GetScene(sceneName))
		{
			scene->SetIsEnabled(false);
		}
	}

	void Scenes::AddScene(Scene* scene, bool setAsCurrentScene)
	{
		if (!scene)
			return;

		for (size_t i = 0; i < m_Scenes.size(); i++)
		{
			if (m_Scenes[i] == scene)
			{
				// delete scene;
				LOG_ERROR("{0} Scene already exists with name {1}", __FUNCTION__);
				return;
			}
		}

		m_Scenes.push_back(scene);
		if (m_CurrentScene == nullptr)
		{
			m_CurrentScene = scene;
			scene->SetIsEnabled(true);
		}
	}

	Scene* Scenes::RemoveScene(Scene* scene)
	{
		for (size_t i = 0; i < m_Scenes.size(); i++)
		{
			if (m_Scenes[i] == scene)
			{
				Scene* returnScene = m_Scenes[i];
				m_Scenes.erase(m_Scenes.begin() + i);
				return returnScene;
			}
		}
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

	void Scenes::DrawScene(std:: string sceneName)
	{
		if (Scene* scene = GetScene(sceneName))
		{
			scene->Draw();
		}
	}

	Scene* Scenes::GetScene(std::string sceneName)
	{
		for (size_t i = 0; i < m_Scenes.size(); i++)
		{
			if (strcmp(m_Scenes[i]->GetSceneName().c_str(), sceneName.c_str()) == 0)
			{
				return m_Scenes[i];
			}
		}
		return nullptr;
	}

}
