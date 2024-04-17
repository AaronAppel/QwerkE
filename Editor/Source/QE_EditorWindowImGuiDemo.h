#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowImGuiDemo : public EditorWindow
		{
		public:
			EditorWindowImGuiDemo() : EditorWindow("ImGui Demo", EditorWindowFlags::Hidden) { }

		private:
			void DrawInternal() override
			{
				bool isOpen = m_WindowFlags & EditorWindowFlags::IsOpen;
				if (isOpen)
				{
					ImGui::ShowDemoWindow(&isOpen);
				}
				u32 result = isOpen ? m_WindowFlags | EditorWindowFlags::IsOpen : m_WindowFlags & ~EditorWindowFlags::IsOpen;
				m_WindowFlags = (EditorWindowFlags)result;
			}
		};

	}

}