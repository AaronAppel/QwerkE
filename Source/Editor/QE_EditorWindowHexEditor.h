#pragma once

#include "Libraries/imgui_hex_editor/imgui_hex.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowHexEditor : public EditorWindow
		{
		public:
			EditorWindowHexEditor(GUID guid);

		private:
			void DrawInternal() override;

			bool m_HexEditorEnabled = false; // #TODO Fix null pointer when imgui de-allocates

			// From: https://github.com/Teselka/imgui_hex_editor
			ImGuiHexEditorState m_HexState;
			ImColor m_UserHighlightColor;
		};

	}

}