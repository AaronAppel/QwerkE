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

	static Scene* s_CurrentScene = nullptr;
	static int s_CurrentSceneIndex = 0;
	static std::vector<Scene*> s_Scenes;
	static std::vector<sPtr<Scene>> s_ScenesSmart; // #TODO Use smart pointers. Deprecate Shutdown

	namespace Scenes
	{

		void priv_UpdateCurrentSceneIndex()
		{
			for (size_t i = 0; i < s_Scenes.size(); i++)
			{
				if (s_Scenes[i] == s_CurrentScene)
				{
					s_CurrentSceneIndex = i;
				}
			}
		}

		void Initialize() // Change to ScenesLoadFromProjectSettings or something more explicit. Should run with 0 scenes
		{
			// Window::OnWindowSizeChange();

			const ProjectSettings& projectSettings = Settings::GetProjectSettings();

			const std::vector<std::string>& sceneFileNames = projectSettings.sceneFileNames;

			for (size_t i = 0; i < sceneFileNames.size(); i++)
			{
				const char* sceneFileName = sceneFileNames[i].c_str();
				if (!FileExists(ScenesFolderPath(sceneFileName)))
				{
					LOG_WARN("Initialize(): File not found: {0}", sceneFileName);
					continue;
				}

				if (Scene* newScene = CreateSceneFromFile(ScenesFolderPath(sceneFileName), false))
				{
					newScene->LoadScene(); // #TODO Should the scene load itself/contents right away?
				}
			}

			if (s_Scenes.empty())
			{
				// Scene* newScene = Scenes::CreateSceneFromFile(NullAssetsFolderPath(null_scene), true);
				Scene* newScene = Scenes::CreateEmptyScene();
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
			s_Scenes.clear();
		}

		void PauseScene(std::string sceneName)
		{
			if (Scene* scene = GetScene(sceneName))
			{
				scene->SetIsPaused(true);
			}
		}

		void UnpauseScene(std::string sceneName)
		{
			if (Scene* scene = GetScene(sceneName))
			{
				scene->SetIsPaused(false);
			}
		}

		void SetCurrentScene(std::string sceneName)
		{
			if (Scene* scene = GetScene(sceneName))
			{
				s_CurrentScene = scene;
				priv_UpdateCurrentSceneIndex();
			}
		}

		void SetCurrentScene(int index)
		{
			if (index >= 0 && index < (int)s_Scenes.size())
			{
				s_CurrentScene = s_Scenes[index];
				priv_UpdateCurrentSceneIndex();
			}
		}

		Scene* CreateSceneFromFile(const std::string& sceneFilePath, bool addToProjectsSceneFiles)
		{
			uPtr<char> sceneFileName = SmartFileName(sceneFilePath.c_str(), true);

			if (const Scene* existingScene = GetScene(sceneFileName.get()))
			{
				LOG_ERROR("{0} Scene already exists with name {1}", __FUNCTION__, sceneFileName.get());
				return nullptr;
			}

			Scene* newScene = new Scene(sceneFileName.get());
			newScene->LoadSceneFromFilePath(sceneFilePath.c_str());
			s_Scenes.push_back(newScene);

			if (s_CurrentScene == nullptr)
			{
				s_CurrentScene = newScene;
				priv_UpdateCurrentSceneIndex();
			}

			if (addToProjectsSceneFiles)
			{
				Settings::GetProjectSettings().sceneFileNames.push_back(newScene->GetSceneName().c_str());
			}

			return newScene;
		}

		Scene* CreateEmptyScene()
		{
			Scene* newScene = new Scene("NewScene");

			while (const Scene* existingScene = GetScene(newScene->GetSceneName()))
			{
				char* newName = NumberAppendOrIncrement(newScene->GetSceneName().c_str());
				if (newName)
				{
					newScene->SetName(newName); // #TODO Handle memory allocations
					delete[] newName;
				}
				else
				{
					LOG_ERROR("{0} Unable to name new game object!", __FUNCTION__);
					delete newScene;
					return nullptr;
				}
			}

			newScene->LoadSceneFromFilePath(NullAssetsFolderPath(null_scene));
			s_Scenes.push_back(newScene);

			if (s_CurrentScene == nullptr)
			{
				s_CurrentScene = newScene;
				priv_UpdateCurrentSceneIndex();
			}

			Settings::GetProjectSettings().sceneFileNames.push_back(newScene->GetSceneName().c_str());

			return newScene;
		}

		Scene* RemoveScene(Scene* scene)
		{
			Scene* returnScene = nullptr;
			for (size_t i = 0; i < s_Scenes.size(); i++)
			{
				if (s_Scenes[i] == scene)
				{
					returnScene = s_Scenes[i];
					s_Scenes.erase(s_Scenes.begin() + i);
					auto projectSettings = Settings::GetProjectSettings();
					for (size_t i = 0; i < projectSettings.sceneFileNames.size(); i++)
					{
						if (strcmp(projectSettings.sceneFileNames[i].c_str(), scene->GetSceneName().c_str()) == 0)
						{
							projectSettings.sceneFileNames.erase(projectSettings.sceneFileNames.begin() + i);
							break;
						}
					}
					break;
				}
			}
			return returnScene;
		}

		void Update(float deltatime)
		{
			PROFILE_SCOPE("Scene Manager Update");

			constexpr size_t numberOfHotkeyedScenes = eKeys::eKeys_F12 - eKeys::eKeys_F1 + 1;
			for (size_t i = 0; i < numberOfHotkeyedScenes; i++)
			{
				if (Input::FrameKeyAction((eKeys)(eKeys::eKeys_F1 + i), eKeyState::eKeyState_Press))
				{
					SetCurrentScene(i);
					break;
				}
			}

			if (s_CurrentScene)
			{
				s_CurrentScene->Update(deltatime);
			}
		}

		void DrawCurrentScene()
		{
			PROFILE_SCOPE("Scene Manager Render");
			if (s_CurrentScene)
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

		int GetCurrentSceneIndex()
		{
			return s_CurrentSceneIndex;
		}

		int SceneCount()
		{
			return (int)s_Scenes.size();
		}

		const std::vector<Scene*>& LookAtScenes()
		{
			return s_Scenes;
		}

		void SetCurrentSceneDirty()
		{
			if (s_CurrentScene)
			{
				s_CurrentScene->SetDirty();
			}
		}

	}

}
