#pragma once

#include "Libraries/imgui/QwerkE_imgui.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowDefaultDebug : public EditorWindow
		{
		public:
			EditorWindowDefaultDebug::EditorWindowDefaultDebug(GUID guid) :
				EditorWindow(ImGui::g_DefaultDebugWindowName,
					EditorWindowTypes::DefaultDebug,
					guid,
					(EditorWindowFlags)(EditorWindowFlags::ExactNameNoguid | EditorWindowFlags::Singleton))
			{ }

			bool IsUnique() override { return true; }

		private:
			void DrawInternal() override
			{
				static u8 m_TextureId = 3;
				ImGui::PushItemWidth(80);
				s32 tempIntTextureId = m_TextureId;
				ImGui::SameLine();
				if (ImGui::InputInt("Texture Id", &tempIntTextureId, 1))
				{
					constexpr s32 maxTextureId = 7;
					m_TextureId = Math::Clamp(0, tempIntTextureId, maxTextureId);
				}
				ImGui::PopItemWidth();
				ImVec2 contentRegionAvailable = { 250 , 250 };
				ImGui::Image(ImTextureID(m_TextureId), contentRegionAvailable, ImVec2(0, 0), ImVec2(1, 1));
			}
		};

	}

}