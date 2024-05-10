#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowMaterialEditor : public EditorWindow
		{
		public:
			EditorWindowMaterialEditor::EditorWindowMaterialEditor(GUID guid = GUID()) :
				EditorWindow("Material Editor", EditorWindowTypes::MaterialEditor, guid) { }

		private:
			void DrawInternal() override
			{
			}
		};

	}

}