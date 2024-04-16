#include "QE_EditorWindowSceneView.h"

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#include "QF_Scene.h"
#include "QF_Scenes.h"

namespace QwerkE {

	namespace Editor {

		EditorWindowSceneView::EditorWindowSceneView(std::string windowName) :
			EditorWindow(windowName)
		{
			m_Flags = ImGuiWindowFlags_NoScrollbar; // | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground
		}

		void EditorWindowSceneView::DrawInternal()
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

	}

}