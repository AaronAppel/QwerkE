#pragma once

#include "QE_EditorWindow.h"

#include "QF_Scenes.h"

namespace QwerkE {

	class Scene;

	namespace Editor {

		class EditorWindowSceneView : public EditorWindow
		{
		public:
			EditorWindowSceneView::EditorWindowSceneView(std::string windowName, u8 viewId = 0, GUID guid = GUID()) :
				EditorWindow(windowName, EditorWindowTypes::SceneView, guid)
			{
				m_ImGuiFlags = ImGuiWindowFlags_NoScrollbar;
			}

			void SetViewId(u8 viewId) { m_ViewId = viewId; }

		private:

			void DrawInternal()
			{
				Scene* currentScene = Scenes::GetCurrentScene();
				if (!currentScene)
					return;

				ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

				ImGui::Image(ImTextureID(m_ViewId), ImVec2(contentRegionAvailable.x, contentRegionAvailable.y), ImVec2(0, 1), ImVec2(1, 0));

				ImGui::DefaultDebugWindow([&]()
				{
					std::string title = "Scene view texture";
					int viewIdInt = m_ViewId;
					if (ImGui::InputInt((title + std::to_string(GetGuid())).c_str(), &viewIdInt))
					{
						m_ViewId = viewIdInt;
					}
				});
			}

			u8 m_ViewId = 0;
		};

	}

}