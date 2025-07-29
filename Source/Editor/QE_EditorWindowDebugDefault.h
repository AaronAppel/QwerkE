#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#include "Libraries/imgui/QwerkE_imgui.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowDebugDefault : public EditorWindow
		{
		public:
			EditorWindowDebugDefault(GUID guid) :
				EditorWindow(ImGui::g_DefaultDebugWindowName,
					EditorWindowTypes::DefaultDebug,
					guid,
					(EditorWindowFlags)(EditorWindowFlags::ExactNameNoguid | EditorWindowFlags::Singleton))
			{
				// #TODO Validate m_TextureId
			}

			bool IsUnique() override { return true; }

		private:
			void DrawInternal() override
			{
				return;

				ImGui::PushItemWidth(80);
				s32 tempIntTextureId = m_TextureId;
				if (ImGui::ScrollerInt("Texture Id", &tempIntTextureId, 1))
				{
					// #TODO List all valid textureIDs from Assets:: and iterate index over the list/array
					constexpr s32 maxTextureId = 100;
					m_TextureId = Math::Clamp(0, tempIntTextureId, maxTextureId);
				}
				ImGui::PopItemWidth();
				ImGui::SliderFloat("Scaler", &m_Scaler, 0.1f, 1.f);
				const float availableWidth = ImGui::GetContentRegionAvail().x * m_Scaler;
				const ImVec2 imageSize = { availableWidth, availableWidth * (9.f / 16.f)};
				ImGui::Image(ImTextureID(m_TextureId), imageSize, ImVec2(0, 0), ImVec2(1, 1));
			}

			MIRROR_PRIVATE_MEMBERS

			u8 m_TextureId = 0;
			float m_Scaler = 1.f;
		};

	}

}