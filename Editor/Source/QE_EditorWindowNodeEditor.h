#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowNodeEditor : public EditorWindow
		{
		public:
            EditorWindowNodeEditor::EditorWindowNodeEditor(GUID guid = GUID()) :
				EditorWindow("Node Editor", EditorWindowTypes::NodeEditor, guid) { }

		private:
			void DrawInternal() override
			{
			}
		};

	}

}