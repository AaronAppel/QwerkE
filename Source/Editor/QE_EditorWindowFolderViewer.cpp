#include "QE_EditorWindowFolderViewer.h"

// #TODO Clean up test code
#define _QICONFONT
#ifdef _QICONFONT
#include "Libraries/IconFont/IconsFontAwesome5.h"
#endif

#include "Libraries/ImFileDialog/ImFileDialog.h"

#include <filesystem>

#include "QF_Paths.h"
#include "QF_Log.h"

namespace QwerkE {

	namespace Editor {

		void local_FileDialog();

		void EditorWindowFolderViewer::DrawInternal()
		{
			// local_FileDialog(); // #TODO Fix ImFileDialog bugs

			std::string path = m_CurrentDirectory.string();
			u16 index = 0;
			for (size_t i = 0; i < path.size(); i++)
			{
				if ('\\' == path[i])
				{
					++i;
					std::string temp = path.substr(index, i - index);
					index = i;
					if (ImGui::Button(temp.c_str()))
					{
						m_CurrentDirectory = path.substr(0, index);
					}
					if (i < path.size() - 1)
					{
						ImGui::SameLine();
					}
				}
				else if (i == path.size() - 1)
				{
					std::string temp = path.substr(index, i - index + 1);
					ImGui::Button(temp.c_str());
				}
			}

			ImGui::SameLineEnd(55.f);
			ImGui::PushItemWidth(150.f);
			// #TODO Add search or filter feature
			ImGui::InputText("##SearchOrFilter", m_SearchOrFilterBuffer.data(), m_SearchOrFilterBuffer.capacity(), ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
			ImGui::PopItemWidth();

			ImGui::SameLineEnd(27.f);
			const bool excludingDirs = m_Filtering & e_ExcludeDirectories;
			if (excludingDirs)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(21.f, 68.05f, 66.27));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(21.f, 68.05f, 66.27f));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.4f, 1.f, 0.4f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.4f, 1.f, 0.4f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.4f, 1.f, 0.4f));
			}
			if (ImGui::SmallButton("Dirs"))
			{
				if (m_Filtering & e_ExcludeDirectories)
				{
					m_Filtering = (FolderViewerFiltering)(m_Filtering ^ e_ExcludeDirectories);
				}
				else
				{
					m_Filtering = (FolderViewerFiltering)(m_Filtering | e_ExcludeDirectories);
				}
			}
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::Text("|");
			ImGui::SameLine();
			const bool excludingFiles = m_Filtering & e_ExcludeFiles;
			if (excludingFiles)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(21.f, 68.05f, 66.27));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(21.f, 68.05f, 66.27f));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.4f, 1.f, 0.4f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.4f, 1.f, 0.4f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.4f, 1.f, 0.4f));
			}
			// #TODO Push style vars
			if (ImGui::SmallButton("Files"))
			{
				if (m_Filtering & e_ExcludeFiles)
				{
					m_Filtering = (FolderViewerFiltering)(m_Filtering ^ e_ExcludeFiles);
				}
				else
				{
					m_Filtering = (FolderViewerFiltering)(m_Filtering | e_ExcludeFiles);
				}
			}
			ImGui::PopStyleColor(3);

			ImGui::SameLineEnd(11.f);
			ImGui::PushItemWidth(100);
			ImGui::SliderFloat("##ScalarSlider", &m_UiScalar, 0.3f, 3.f, "Scale", ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
			ImGui::PopItemWidth();

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
					if (excludingDirs)
					{
						continue;
					}
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.4f, 1.f, 0.4f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.4f, 1.f, 0.4f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.4f, 1.f, 0.4f));
				}
				else
				{
					if (excludingFiles)
					{
						continue;
					}
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

		void local_FileDialog()
		{
			// #TODO Fix m_getIcon->CreateTexture() runtime crash
			// Review: https://github.com/dfranx/ImFileDialog#usage
			return;

			// Simple window
			ImGui::Begin("Control Panel");
			if (ImGui::Button("Open file"))
				ifd::FileDialog::Instance().Open("ShaderOpenDialog", "Open a shader", "Image file (*.png;*.jpg;*.jpeg;*.bmp;*.tga){.png,.jpg,.jpeg,.bmp,.tga},.*", true);
			if (ImGui::Button("Open directory"))
				ifd::FileDialog::Instance().Open("DirectoryOpenDialog", "Open a directory", "");
			if (ImGui::Button("Save file"))
				ifd::FileDialog::Instance().Save("ShaderSaveDialog", "Save a shader", "*.sprj {.sprj}");
			ImGui::End();

			// File dialogs
			if (ifd::FileDialog::Instance().IsDone("ShaderOpenDialog")) {
				if (ifd::FileDialog::Instance().HasResult()) {
					const std::vector<std::filesystem::path>& res = ifd::FileDialog::Instance().GetResults();
					for (const auto& r : res) // ShaderOpenDialog supports multiselection
						printf("OPEN[%s]\n", r.u8string().c_str());
				}
				ifd::FileDialog::Instance().Close();
			}
			if (ifd::FileDialog::Instance().IsDone("DirectoryOpenDialog")) {
				if (ifd::FileDialog::Instance().HasResult()) {
					std::string res = ifd::FileDialog::Instance().GetResult().u8string();
					printf("DIRECTORY[%s]\n", res.c_str());
				}
				ifd::FileDialog::Instance().Close();
			}
			if (ifd::FileDialog::Instance().IsDone("ShaderSaveDialog")) {
				if (ifd::FileDialog::Instance().HasResult()) {
					std::string res = ifd::FileDialog::Instance().GetResult().u8string();
					printf("SAVE[%s]\n", res.c_str());
				}
				ifd::FileDialog::Instance().Close();
			}
		}

	}

}