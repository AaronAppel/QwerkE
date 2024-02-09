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

    bool s_IsRunning = true;
    Scene* s_CurrentScene = nullptr;
    std::vector<Scene*> s_Scenes;

	namespace Scenes
	{

		void Initialize()
		{
			const ProjectSettings& projectSettings = Settings::GetProjectSettings();

			// #TODO How should the file names be given to the scene?
			const std::vector<std::string>& sceneFileNames = projectSettings.sceneFileNames;

			for (size_t i = 0; i < sceneFileNames.size(); i++)
			{
				const char* sceneFileName = sceneFileNames[i].c_str();
				if (FileExists(ScenesFolderPath(sceneFileName)) == false)
				{
					LOG_WARN("Initialize(): File not found: {0}", sceneFileName);
					continue;
				}

				Scene* newScene = new Scene(sceneFileName);
				newScene->LoadScene();
				AddScene(newScene, i == 0); // #TODO Improve default starting scene selection/specification
			}

			if (s_Scenes.empty())
			{
				Scene* emptyScene = new Scene(null_scene);
				emptyScene->LoadScene();
				emptyScene->SetIsEnabled(true);
				AddScene(emptyScene, true);
				LOG_WARN("Null scene loaded as no scene files found for project: {0}", projectSettings.projectName);
			}
		}

		void Shutdown()
		{
			for (size_t i = 0; i < s_Scenes.size(); i++)
			{
				if (s_Scenes[i])
				{
					delete s_Scenes[i];
				}
			}
		}

		void EnableScene(std::string sceneName)
		{
			if (Scene* scene = GetScene(sceneName))
			{
				scene->SetIsEnabled(true);
			}
		}

		void SetCurrentScene(std::string sceneName)
		{
			if (Scene* scene = GetScene(sceneName))
			{
				s_CurrentScene = scene;
			}
		}

		void DisableScene(std::string sceneName)
		{
			if (Scene* scene = GetScene(sceneName))
			{
				scene->SetIsEnabled(false);
			}
		}

		void AddScene(Scene* scene, bool setAsCurrentScene)
		{
			if (!scene)
				return;

			for (size_t i = 0; i < s_Scenes.size(); i++)
			{
				if (s_Scenes[i] == scene)
				{
					// delete scene;
					LOG_ERROR("{0} Scene already exists with name {1}", __FUNCTION__);
					return;
				}
			}

			s_Scenes.push_back(scene);
			if (s_CurrentScene == nullptr)
			{
				s_CurrentScene = scene;
				scene->SetIsEnabled(true);
			}
		}

		Scene* RemoveScene(Scene* scene)
		{
			for (size_t i = 0; i < s_Scenes.size(); i++)
			{
				if (s_Scenes[i] == scene)
				{
					Scene* returnScene = s_Scenes[i];
					s_Scenes.erase(s_Scenes.begin() + i);
					return returnScene;
				}
			}
		}

		void Update(float deltatime)
		{
			PROFILE_SCOPE("Scene Manager Update");

			if (Input::FrameKeyAction(eKeys::eKeys_P, eKeyState::eKeyState_Press))
			{
				GetCurrentScene()->ToggleIsPaused();
			}

			if (s_CurrentScene && s_IsRunning && s_CurrentScene->GetIsEnabled())
			{
				s_CurrentScene->Update(deltatime);
			}
		}

		void DrawCurrentScene()
		{
			PROFILE_SCOPE("Scene Manager Render");
			if (s_CurrentScene && s_CurrentScene->GetIsEnabled())
			{
				s_CurrentScene->Draw();
			}
		}

		void DrawScene(std::string sceneName)
		{
			if (Scene* scene = GetScene(sceneName))
			{
				scene->Draw();
			}
		}

		Scene* GetScene(std::string sceneName)
		{
			for (size_t i = 0; i < s_Scenes.size(); i++)
			{
				if (strcmp(s_Scenes[i]->GetSceneName().c_str(), sceneName.c_str()) == 0)
				{
					return s_Scenes[i];
				}
			}
			return nullptr;
		}

		Scene* GetCurrentScene()
		{
			return s_CurrentScene;
		}

		int SceneCount()
		{
			return (int)s_Scenes.size();
		}

		const std::vector<Scene*>& LookAtScenes()
		{
			return s_Scenes;
		}
	}

}
