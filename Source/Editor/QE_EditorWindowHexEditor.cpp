#include "QE_EditorWindowHexEditor.h"

#include "Libraries/imgui/imgui.h"

namespace QwerkE {

	namespace Editor {

        EditorWindowHexEditor::EditorWindowHexEditor(GUID guid) :
            EditorWindow("Hex Editor", EditorWindowTypes::HexEditor, guid)
        {
            if (!m_HexEditorEnabled)
                return;

            m_HexState.ReadCallback = [](ImGuiHexEditorState* state, int offset, void* buf, int size) -> int {
                SIZE_T read;
                ReadProcessMemory(GetCurrentProcess(), (char*)state->Bytes + offset, buf, size, &read);
                return read;
                };

            m_HexState.WriteCallback = [](ImGuiHexEditorState* state, int offset, void* buf, int size) -> int {
                SIZE_T write;
                WriteProcessMemory(GetCurrentProcess(), (char*)state->Bytes + offset, buf, size, &write);
                return write;
                };

            m_HexState.GetAddressNameCallback = [](ImGuiHexEditorState* state, int offset, char* buf, int size) -> bool
                {
                    if (offset >= 0 && offset < sizeof(ImGuiIO))
                    {
                        snprintf(buf, size, "io+%0.*zX", 4, offset);
                        return true;
                    }

                    return false;
                };

            m_HexState.SingleHighlightCallback = [](ImGuiHexEditorState* state, int offset, ImColor* color, ImColor* text_color, ImColor* border_color) -> ImGuiHexEditorHighlightFlags
                {
                    if (offset >= 100 && offset <= 150)
                    {
                        *color = ImColor(); // Could not pass user_highlight_color into lambda by reference
                        return ImGuiHexEditorHighlightFlags_Apply | ImGuiHexEditorHighlightFlags_TextAutomaticContrast | ImGuiHexEditorHighlightFlags_Ascii
                            | ImGuiHexEditorHighlightFlags_BorderAutomaticContrast;
                    }

                    return ImGuiHexEditorHighlightFlags_None;
                };

            m_HexState.HighlightRanges.clear();

            {
                ImGuiHexEditorHighlightRange range;
                range.From = 200;
                range.To = 250;
                range.Color = ImColor(m_UserHighlightColor);
                range.Flags = ImGuiHexEditorHighlightFlags_TextAutomaticContrast | ImGuiHexEditorHighlightFlags_FullSized
                    | ImGuiHexEditorHighlightFlags_Ascii | ImGuiHexEditorHighlightFlags_Border | ImGuiHexEditorHighlightFlags_BorderAutomaticContrast;
                m_HexState.HighlightRanges.push_back(range);
            }

            m_HexState.Bytes = (void*)&ImGui::GetIO();
            m_HexState.MaxBytes = sizeof(ImGuiIO) + 0x1000;
        }

        void EditorWindowHexEditor::DrawInternal()
        {
            ImGui::Text("Fix allocator bug!");
            if (!m_HexEditorEnabled)
                return;

            if (ImGui::Begin("Hex Editor##Window"))
            {
                ImGui::BeginHexEditor("Hex Editor", &m_HexState);
                ImGui::EndHexEditor();
            }
            ImGui::End();

        }

	}

}