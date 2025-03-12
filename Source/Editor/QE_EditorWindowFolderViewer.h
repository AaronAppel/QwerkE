#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#include <filesystem>

#include "QF_Paths.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowFolderViewer : public EditorWindow
		{
		public:
			EditorWindowFolderViewer::EditorWindowFolderViewer(GUID guid = GUID()) :
				EditorWindow("Folder Viewer", EditorWindowTypes::FolderViewer, guid) { }

		private:
			void DrawInternal() override
			{
				ImGui::Text(m_CurrentDirectory.string().c_str());

				ImGui::SameLineEnd(20);
				ImGui::PushItemWidth(100);
				ImGui::SliderFloat("##ScalarSlider", &m_UiScalar, 0.3f, 3.f, "Scale", ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
				ImGui::PopItemWidth();

				ImGui::SameLineEnd((u16)0);
				if (ImGui::Button(" <- "))
				{
					m_CurrentDirectory = m_CurrentDirectory.parent_path();
				}

				constexpr float padding = 5.f;
				float thumbnailSize = 128.f * m_UiScalar;
				float cellSize = thumbnailSize + padding;

				float panelWidth = ImGui::GetContentRegionAvail().x;
				int columnCount = (int)(panelWidth / cellSize);
				if (columnCount < 1)
					columnCount = 1;

				ImGui::Columns(columnCount, 0, false);

				for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
				{
					// #NOTE Crashes when viewing file names containing emojis
					const auto& path = directoryEntry.path();
					std::string filenameString = path.filename().string();

					if (ImGui::Button(filenameString.c_str(), { thumbnailSize, thumbnailSize }))
					{
						if (directoryEntry.is_directory())
							m_CurrentDirectory /= path.filename();
					}

					ImGui::NextColumn();
				}
			}

			MIRROR_PRIVATE_MEMBERS

			float m_UiScalar = 1.f;
			Path m_CurrentDirectory = Paths::AssetsDir();
		};

	}

}