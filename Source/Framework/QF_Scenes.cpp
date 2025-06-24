#include "QF_Scenes.h"

#include <vector>

#include "QF_Mirror.h" // For Mirror::IdForType<x>()

#include "QF_Assets.h"
#include "QF_PathDefines.h"
#include "QF_Enums.h"
#include "QF_Files.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Paths.h"
#include "QF_Profile.h"
#include "QF_Scene.h"

#include "../Source/Editor/QE_Projects.h" // #TODO Remove from QF_* domain
#include "../Source/Editor/QE_Settings.h" // #TODO Remove from QF_* domain

namespace QwerkE {

	static bool s_Initialized = false;
	static Scene* s_CurrentScene = nullptr; // #NOTE Scene transition changes removes line
	static u32 s_CurrentSceneIndex = 0; // #NOTE Scene transition changes removes line
	static std::vector<Scene*> s_Scenes;
	// static std::vector<sPtr<Scene>> s_ScenesSmart; // #TODO Use smart pointers. Deprecate Shutdown

	namespace Scenes
	{
		// #NOTE Scene transition changes removes:
		// void local_UpdateCurrentSceneIndex(), void local_UpdateCurrentScene(), and void Initialize()
		void local_UpdateCurrentSceneIndex()
		{
			for (u32 i = 0; i < s_Scenes.size(); i++)
			{
				if (s_Scenes[i] == s_CurrentScene)
				{
					s_CurrentSceneIndex = i;
				}
			}
		}

		void local_UpdateCurrentScene()
		{
			if (!GetScene(s_CurrentScene))
			{
				s_CurrentScene = nullptr;
				if (!s_Scenes.empty())
				{
					s_CurrentScene = s_Scenes[0];
				}
			}

			local_UpdateCurrentSceneIndex();
		}

		void Initialize()
		{
			// #NOTE Kept for consistent API with other systems
			s_Initialized = true;
		}

		void Shutdown()
		{
			// #TODO Handle loaded scenes in Assets
			// Assets::

			for (size_t i = 0; i < s_Scenes.size(); i++)
			{
				if (s_Scenes[i])
				{
					delete s_Scenes[i];
				}
			}
			s_Scenes.clear();
			s_CurrentScene = nullptr; // #NOTE Scene transition changes removes line
			s_CurrentSceneIndex = 0; // #NOTE Scene transition changes removes line
			s_Initialized = false;
		}

		// #NOTE Scene transition changes removes SetScenePaused(), SetCurrentScene(Scene* scene), void SetCurrentScene(const GUID& sceneGuid)
		void SetScenePaused(const GUID& sceneGuid, const bool isPaused)
		{
			if (Scene* scene = GetScene(sceneGuid))
			{
				scene->SetIsPaused(isPaused);
			}
		}

		void SetCurrentScene(Scene* scene)
		{
			for (size_t i = 0; i < s_Scenes.size(); i++)
			{
				// #NOTE Scene transition changes
				// if (s_Scenes[i]->GetSceneName() == sceneFileName)

				if (s_Scenes[i] == scene)
				{
					s_CurrentScene = s_Scenes[i];
					return;

					// #NOTE Scene transition changes
					// LOG_ERROR("{0} Scene already exists with name {1}", __FUNCTION__, sceneFileName.string().c_str());
					// return nullptr;
				}
			}
		}

		void SetCurrentScene(const GUID& sceneGuid)
		{
			if (Scene* scene = GetScene(sceneGuid))
			{
				s_CurrentScene = scene;
				local_UpdateCurrentSceneIndex();
			}
		}

		Scene* CreateSceneFromFile(const std::string& sceneFilePath)
		{
			// #NOTE Scene transition change
			// Path sceneFileName = Files::FileName(Paths::Scene(sceneFilePath.c_str()).c_str());

			// Scene* newScene = new Scene(sceneFileName.string().c_str());
			// newScene->LoadSceneFromFilePath(sceneFilePath.c_str());
			// s_Scenes.push_back(newScene);

			// return newScene;

			// #TODO Validate result
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
				local_UpdateCurrentSceneIndex();
			}
			return newScene;
		}

		Scene* CreateScene(const char* const sceneFileName)
		{
			ASSERT(sceneFileName, "Null argument passed!");

			const Path uniqueSceneFilePath = Files::UniqueFilePath(Paths::ScenesDir().c_str(), sceneFileName);
			const Path uniqueFileNamePath = Files::FileName(uniqueSceneFilePath.string().c_str());
			std::string uniqueFileName = uniqueFileNamePath.string();

			// #NOTE Scene transition change
			// const AssetsList& scenesRegistry = Assets::GetRegistryAssetList(Mirror::IdForType<Scene>());
			// for (size_t i = 0; i < s_Scenes.size(); i++)
			// {
			// 	if (s_Scenes[i]->GetSceneName() == sceneFileName)
			// 	{
			// 		LOG_ERROR("{0} Scene exists with same name {1}", __FUNCTION__, uniqueFileName.c_str());
			// 		// #TODO Decide how to handle case where file was deleted, but scene with same name is still loaded.
			// 		// Can find a new and more valid scene name, guaranteeing a valid scene is created and a pointer returned.
			// 		// while(GetScene(uniqueFileName.c_str()) uniqueFileName = NumberAppendOrIncrement(uniqueFileName.c_str());
			// 		return nullptr;
			// 	}
			// }

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

			s_CurrentScene = newScene; // #NOTE Scene transition changes removed line
			local_UpdateCurrentSceneIndex(); // #NOTE Scene transition changes removed line

			return newScene;
		}

		Scene* CreateScene(const GUID& sceneGuid)
		{
			const AssetsList& scenesRegistry = Assets::GetRegistryAssetList(Mirror::IdForType<Scene>()); // #NOTE Scene transition changes removed line
			std::string sceneFileName; // #NOTE Scene transition changes removed line
			for (size_t i = 0; i < scenesRegistry.size(); i++) // #NOTE Scene transition changes removed line
			{
				auto guidStringPair = scenesRegistry[i];
				if (sceneGuid == guidStringPair.first)
				{
					// #TODO Decide how to search for shader and materials that have more than 1 string in vector
					sceneFileName = guidStringPair.second[0];
					break;
				}
			}

			// #NOTE Scene transition changes
			// #TODO Check if exists? Duplicate GUIDs is not allowed, but creating a new scene from an existing scene is valid
			// std::string sceneFileName = Assets::GetRegistryAssetFileName<Scene>(sceneGuid);
			// if (!sceneFileName.empty())

			return CreateSceneFromFile(Paths::Scene(sceneFileName.c_str()));
		}

		Scene* CreateScene(const std::string& a_SceneFileName) // #TODO Define API for creating a new, empty scene
		{
			// #TODO Error checking, and how should the registry learn about the new file?
			Scene* newScene = new Scene();
			newScene->m_SceneFileName = a_SceneFileName;
			newScene->SaveScene();

			GUID guid = newScene->GetGuid();

			// delete newScene; // #TODO Improve new scene Assets:: loading

			// AssetsList& scenesRegistry = Assets::GetRegistryAssetList(Mirror::IdForType<Scene>()); // #NOTE Scene transition changes removed line
			Assets::Add<Scene>(newScene, guid);
			Assets::AddToRegistry(Mirror::IdForType<Scene>(), guid, a_SceneFileName);
			Scene* getScene = Assets::Get<Scene>(guid);
			if (const std::unordered_map<GUID, Scene*>* scenes = Assets::ViewAssets<Scene>())
			{
				int bp = 0;
			}

			LOG_TRACE("{0} New Scene {1} created with GUID {2}", __FUNCTION__, newScene->GetSceneName(), newScene->GetGuid());
			s_Scenes.push_back(newScene);
			return newScene;
		}

		bool SceneExists(const char* const sceneFileName)
		{
			if (!sceneFileName)
			{
				return false;
			}
			return nullptr != GetScene(sceneFileName);
		}

		bool SceneExists(const GUID& a_Guid)
		{
			if (GUID::Invalid == a_Guid)
			{
				return false;
			}
			return nullptr != GetScene(a_Guid);
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

				LOG_INFO("{0} Destroying scene {1}", __FUNCTION__, scene->GetSceneName().c_str());

				s_Scenes[i]->UnloadScene();
				s_Scenes.erase(s_Scenes.begin() + i);

				LOG_INFO("{0} Scene destroyed", __FUNCTION__);
				break;
			}
			local_UpdateCurrentScene();
		}

		// #NOTE Scene transition removes void Update(float deltatime), void DrawCurrentScene(u16 viewId), void DrawScene(std::string sceneName)
		void Update(float deltatime)
		{
			PROFILE_SCOPE("Scene Manager Update");

			if (s_CurrentScene)
			{
				s_CurrentScene->Update(deltatime);
			}
		}

		void DrawCurrentScene(u16 viewId)
		{
			PROFILE_SCOPE("Scene Manager Render");
			if (s_CurrentScene)
			{
				s_CurrentScene->Draw(viewId);
			}
		}

		void DrawScene(std::string sceneName)
		{
			if (Scene* scene = GetScene(sceneName))
			{
				scene->Draw();
			}
		}

		// #NOTE Scene transition changes
		// void UpdateScenes(float deltatime)
		// {
		// 	PROFILE_SCOPE("Scenes Update");
		// 	for (size_t i = 0; i < s_Scenes.size(); i++)
		// 	{
		// 		s_Scenes[i]->Update(deltatime);
		// 	}
		// }
		// void DrawScenes(u16 viewId)
		// {
		// 	PROFILE_SCOPE("Scene Render");
		// 	for (size_t i = 0; i < s_Scenes.size(); i++)
		// 	{
		// 		s_Scenes[i]->Draw(viewId);
		// 	}
		// }

		Scene* GetScene(const GUID& guid)
		{
			for (size_t i = 0; i < s_Scenes.size(); i++)
			{
				if (s_Scenes[i]->GetGuid() == guid)
				{
					return s_Scenes[i];
				}
			}
			return nullptr;
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

		// #TODO This really just checks if the scene is still in s_Scenes, so could return a bool and name IsSceneInList or something
		Scene* GetScene(const Scene* const scene)
		{
			NULL_ARG_CHECK_RETURN(scene, nullptr);

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

		// #NOTE Scene transition change removes GetCurrentScene()
		Scene* GetCurrentScene()
		{
			return s_CurrentScene;
		}

		int GetCurrentSceneIndex()
		{
			return s_CurrentSceneIndex;
		}

		// #NOTE Scene transition change
		// bool HasScene(const Scene* scene)
		// {
		// 	return GetScene(scene->GetGuid()) != nullptr;
		// }

		int SceneCount()
		{
			return (int)s_Scenes.size();
		}

		const std::vector<Scene*>& LookAtScenes()
		{
			return s_Scenes;
		}

		// #NOTE Scene transition change removes SetCurrentSceneDirty()
		void SetCurrentSceneDirty()
		{
			if (s_CurrentScene)
			{
				s_CurrentScene->SetDirty();
			}
		}

	}

}
