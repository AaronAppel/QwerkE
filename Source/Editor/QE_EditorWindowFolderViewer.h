#pragma once

// #TODO Clean up test code
#define _QICONFONT
#ifdef _QICONFONT
#include "Libraries/IconFont/IconsFontAwesome5.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#include <filesystem>

#include "QF_Paths.h"
#include "QF_Log.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		// #TODO Consider re-naming ...DirectoryViewer
		class EditorWindowFolderViewer : public EditorWindow
		{
		public:
			EditorWindowFolderViewer::EditorWindowFolderViewer(GUID guid = GUID()) :
				EditorWindow("Folder Viewer", EditorWindowTypes::FolderViewer, guid)
			{
				// #TODO Clean up test code

			}

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

					if (directoryEntry.is_directory())
					{
						ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.4f, 1.f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.4f, 1.f, 0.4f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.4f, 1.f, 0.4f));
					}
					else
					{
						ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(21.f, 68.05f, 66.27));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(21.f, 68.05f, 66.27f));
					}

					if (ImGui::Button(filenameString.c_str(), { thumbnailSize, thumbnailSize }))
					{
						// #TODO Use images or icon fonts from: https://github.com/juliettef/IconFontCppHeaders
						if (directoryEntry.is_directory())
						{
							m_CurrentDirectory /= path.filename();
						}
						else
						{
							// #TODO Try to open file
						}
					}

					if (ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("Any", filenameString.c_str(), filenameString.size() + 1);
						ImGui::EndDragDropSource();
					}

					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Any"))
						{
							LOG_INFO("{0}", (char*)payload->Data);
						}
						ImGui::EndDragDropTarget();
					}

					ImGui::PopStyleColor(3);
					ImGui::NextColumn();
				}
			}

			MIRROR_PRIVATE_MEMBERS

			float m_UiScalar = 1.f;
			Path m_CurrentDirectory = Paths::AssetsDir();
		};

	}

}