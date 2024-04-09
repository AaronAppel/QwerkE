#include "QF_Scenes.h"

#include <vector>

#include "QF_PathDefines.h"
#include "QF_Enums.h"
#include "QF_Files.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Paths.h"
#include "QF_Profile.h"
#include "QF_Projects.h"
#include "QF_Scene.h"
#include "QF_Settings.h"

namespace QwerkE {

	static Scene* s_CurrentScene = nullptr;
	static u32 s_CurrentSceneIndex = 0;
	static std::vector<Scene*> s_Scenes;
	// static std::vector<sPtr<Scene>> s_ScenesSmart; // #TODO Use smart pointers. Deprecate Shutdown

	namespace Scenes
	{

		void priv_UpdateCurrentSceneIndex()
		{
			for (u32 i = 0; i < s_Scenes.size(); i++)
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

			const Project& project = Projects::CurrentProject();

			const std::vector<std::string>& sceneFileNames = project.sceneFileNames;

			for (size_t i = 0; i < sceneFileNames.size(); i++)
			{
				const char* sceneFileName = sceneFileNames[i].c_str();
				if (!Files::Exists(Paths::Scene(sceneFileName).c_str()))
				{
					LOG_WARN("Initialize(): File not found: {0}", sceneFileName);
					continue;
				}

				CreateSceneFromFile(Paths::Scene(sceneFileName).c_str(), false);
			}

			if (s_Scenes.empty())
			{
				CreateSceneFromFile(Paths::NullAsset(null_scene), false);
				LOG_WARN("Null scene loaded as no scene files found for project: {0}", project.projectName);
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

		void SetCurrentScene(Scene* scene)
		{
			for (size_t i = 0; i < s_Scenes.size(); i++)
			{
				if (s_Scenes[i] == scene)
				{
					s_CurrentScene = s_Scenes[i];
					return;
				}
			}
		}

		Scene* CreateSceneFromFile(const std::string& sceneFilePath, bool addToProjectsSceneFiles)
		{
			Path sceneFileName = Files::FileName(sceneFilePath.c_str());

			if (const Scene* existingScene = GetScene(sceneFileName.string().c_str()))
			{
				LOG_ERROR("{0} Scene already exists with name {1}", __FUNCTION__, sceneFileName.string().c_str());
				return nullptr;
			}

			Scene* newScene = new Scene(sceneFileName.string().c_str());
			newScene->LoadSceneFromFilePath(sceneFilePath.c_str());
			s_Scenes.push_back(newScene);

			if (s_CurrentScene == nullptr)
			{
				s_CurrentScene = newScene;
				priv_UpdateCurrentSceneIndex();
			}

			if (addToProjectsSceneFiles)
			{
				Projects::CurrentProject().sceneFileNames.emplace_back(newScene->GetSceneName().c_str());
			}

			return newScene;
		}

		Scene* CreateScene(const char* const sceneFileName, bool addToProjectsSceneFiles)
		{
			ASSERT(sceneFileName, "Null argument passed!");

			Path uniqueSceneFilePath = Files::UniqueFilePath(Paths::ScenesDir().c_str(), sceneFileName);
			Path uniqueFileNamePath = Files::FileName(uniqueSceneFilePath.string().c_str());
			std::string uniqueFileName = uniqueFileNamePath.string();

			if (const Scene* existingScene = GetScene(uniqueFileName.c_str()))
			{
				LOG_ERROR("{0} Scene exists with same name {1}", __FUNCTION__, uniqueFileName.c_str());
				// #TODO Decide how to handle case where file was deleted, but scene with same name is still loaded.
				// Can find a new and more valid scene name, guaranteeing a valid scene is created and a pointer returned.
				// while(GetScene(uniqueFileName.c_str()) uniqueFileName = NumberAppendOrIncrement(uniqueFileName.c_str());
				return nullptr;
			}

			Scene* newScene = new Scene(uniqueFileName.c_str());
			newScene->LoadSceneFromFilePath(Paths::NullAsset(null_scene).c_str());
			s_Scenes.push_back(newScene);

			s_CurrentScene = newScene;
			priv_UpdateCurrentSceneIndex();

			if (addToProjectsSceneFiles)
			{
				Projects::CurrentProject().sceneFileNames.emplace_back(newScene->GetSceneName().c_str());
				newScene->SetDirty();
			}

			return newScene;
		}

		void DestroyScene(const Scene* const scene)
		{
			if (!scene || !GetScene(scene))
			{
				LOG_ERROR("{0} Could not destroy scene!", __FUNCTION__);
				return;
			}

			for (size_t i = 0; i < s_Scenes.size(); i++)
			{
				if (s_Scenes[i] != scene)
					continue;

				s_Scenes.erase(s_Scenes.begin() + i);

				Project& project = Projects::CurrentProject();
				for (size_t i = 0; i < project.sceneFileNames.size(); i++)
				{
					if (strcmp(project.sceneFileNames[i].c_str(), scene->GetSceneName().c_str()) == 0)
					{
						project.sceneFileNames.erase(project.sceneFileNames.begin() + i);
						break;
					}
				}
				break;
			}
		}

		void Update(float deltatime)
		{
			PROFILE_SCOPE("Scene Manager Update");

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

		Scene* GetScene(const Scene* const scene)
		{
			NULL_ARG_CHECK_RETURN(scene, nullptr)

			for (size_t i = 0; i < s_Scenes.size(); i++)
			{
				if (scene == s_Scenes[i])
				{
					return s_Scenes[i];
				}
			}
			LOG_WARN("{0} Scene not found", __FUNCTION__);
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
