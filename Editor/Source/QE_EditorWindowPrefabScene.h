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
				// 	Projects::CurrentProject().sceneFileNames.emplace_back(newScene->GetSceneName().c_str());
				// }
			}

			~EditorWindowPrefabScene()
			{
				// Scenes::DestroyScene(m_PrefabScene);
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