#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowShaderEditor : public EditorWindow
		{
		public:
			EditorWindowShaderEditor::EditorWindowShaderEditor(GUID guid = GUID()) : EditorWindow("Shader Editor", EditorWindowTypes::ShaderEditor, guid) { }

		private:
			void DrawInternal() override
			{
			}
		};

	}

}