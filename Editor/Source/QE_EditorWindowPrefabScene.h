#pragma once

#include "QC_Guid.h"

#include "QF_Scene.h"
#include "QF_Scenes.h"

#include "QE_Editor.h"
#include "QE_EditorWindow.h"
#include "QE_Projects.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowPrefabScene : public EditorWindow
		{
		public:
			EditorWindowPrefabScene(GUID guid = GUID()) : EditorWindow("Prefab Scene", EditorWindowTypes::PrefabScene, guid)
			{
				// #TODO Implement EditorWindowPrefabScene
				// if (Scene* newScene = Scenes::CreateScene("PrefabScene_"))
				// {
				// #TODO Move unique check logic into Projects::AddSceneToProjectScenesList() or something better
				// for (size_t i = 0; i < s_CurrentProject.sceneFileNames.size(); i++)
				// {
				// 	if (strcmp(s_CurrentProject.sceneFileNames[i].c_str(), newScene->GetSceneName().c_str()) == 0)
				// 	{
				// 		break;
				// 	}
				// 	else if (i == s_CurrentProject.sceneFileNames.size() - 1)
				// 	{
				// 		s_CurrentProject.sceneFileNames.emplace_back(newScene->GetSceneName().c_str());
				// 	}
				//
				// }
			}

			~EditorWindowPrefabScene()
			{
				// Scenes::DestroyScene(m_PrefabScene);
				// Project& project = Projects::CurrentProject();
				// for (size_t i = 0; i < project.sceneFileNames.size(); i++)
				// {
				// 	if (strcmp(project.sceneFileNames[i].c_str(), scene->GetSceneName().c_str()) == 0)
				// 	{
				// 		project.sceneFileNames.erase(project.sceneFileNames.begin() + i);
				// 		break;
				// 	}
				// }
			}

		private:
			void DrawInternal() override
			{
				if (!m_PrefabScene)
					return;
			}

			Scene* m_PrefabScene = nullptr;
		};

	}

}