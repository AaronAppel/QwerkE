#pragma once

#include <string>

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowStatusBar : public EditorWindow
		{
		public:
			EditorWindowStatusBar(GUID guid) :
				EditorWindow("Status Bar",
					EditorWindowTypes::StatusBar,
					guid,
					(EditorWindowFlags)(EditorWindowFlags::HideWindowOptions))
			{
				m_StatusText = m_StatusPrefix;
				m_StatusText += "Example status message";

				m_RightText = "Right bottom text";
			}

		private:
			void DrawInternal() override
			{
				ImGui::SameLine(ImGui::g_pixelsPerCharacter * 2.f); // #NOTE Two chars for left spacing
				ImGui::Text(m_StatusText.c_str());

				const float charCount = m_RightText.size() + 1.f; // #NOTE Extra char for right spacing
				ImGui::SameLine(ImGui::GetWindowWidth() - (charCount * ImGui::g_pixelsPerCharacter));
				ImGui::Text(m_RightText.c_str());
			}

			const char* const m_StatusPrefix = "Status: ";
			std::string m_StatusText;

			std::string m_RightText;
		};

	}

}
