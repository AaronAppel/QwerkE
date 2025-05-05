#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#include "QF_Paths.h"

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
				m_SearchOrFilterBuffer.reserve(250);
				m_SearchOrFilterBuffer = "Search/filter";
			}

		private:
			void DrawInternal() override;

			MIRROR_PRIVATE_MEMBERS

			enum FolderViewerFiltering : u8
			{
				e_None = 0,
				e_ExcludeDirectories,
				e_ExcludeFiles,
			};
			FolderViewerFiltering m_Filtering = e_None;
			float m_UiScalar = 1.f;
			std::string m_SearchOrFilterBuffer;
			Path m_CurrentDirectory = Paths::AssetsDir();
		};

	}

}