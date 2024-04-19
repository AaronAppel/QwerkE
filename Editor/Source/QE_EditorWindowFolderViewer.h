#pragma once

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
			}
		};

	}

}