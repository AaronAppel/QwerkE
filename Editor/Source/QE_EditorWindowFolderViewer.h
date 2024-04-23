#pragma once

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

				ImGui::SameLineEnd();
				if (ImGui::Button("<-"))
				{
					if (m_CurrentDirectory.string().find('\\', 0) != std::string::npos)
					{
						m_CurrentDirectory = m_CurrentDirectory.parent_path();
					}
				}

				static float padding = 16.0f;
				static float thumbnailSize = 128.0f;
				float cellSize = thumbnailSize + padding;

				float panelWidth = ImGui::GetContentRegionAvail().x;
				int columnCount = (int)(panelWidth / cellSize);
				if (columnCount < 1)
					columnCount = 1;

				ImGui::Columns(columnCount, 0, false);

				for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
				{
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

			Path m_CurrentDirectory = Paths::AssetsDir();
		};

	}

}