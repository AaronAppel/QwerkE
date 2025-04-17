#pragma once

#include "Libraries/imgui-node-editor/blueprints-example.h"

#include "QE_EditorWindow.h"

namespace ax {
	namespace NodeEditor {
		struct EditorContext;
	}
}

namespace QwerkE {

	namespace Editor {

		class EditorWindowNodeEditor : public EditorWindow
		{
		public:
            EditorWindowNodeEditor::EditorWindowNodeEditor(GUID guid = GUID());
            EditorWindowNodeEditor::~EditorWindowNodeEditor();

		private:
			void DrawInternal() override;

            ax::NodeEditor::EditorContext* m_Context = nullptr;
            const char* m_SettingsFileName = "NodeEditorWidgets.json";
			s32 m_EditorStyle = 0;
		};

	}

}