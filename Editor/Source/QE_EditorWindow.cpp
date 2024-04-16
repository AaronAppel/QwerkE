#include "QE_EditorWindow.h"

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif


namespace QwerkE {

	namespace Editor {

		void EditorWindow::Draw()
		{
			if (ImGui::Begin(m_WindowName.c_str()))
			{
				DrawInternal();
			}
			ImGui::End();
		}

	}

}