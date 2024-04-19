#include "QE_EditorWindowNodeEditor.h"

#ifdef _QIMGUINODEEDITOR
#include "Libraries/imgui/imgui_internal.h"
#include "Libraries/imgui-node-editor/imgui_node_editor.h"
#endif

namespace QwerkE {

	namespace Editor {

        EditorWindowNodeEditor::EditorWindowNodeEditor(GUID guid) :
            EditorWindow("Node Editor", EditorWindowTypes::NodeEditor, guid)
        {
            ax::NodeEditor::Config config;
            config.SettingsFile = m_SettingsFileName;
            m_Context = ax::NodeEditor::CreateEditor(&config);
        }

        EditorWindowNodeEditor::~EditorWindowNodeEditor()
        {
            ax::NodeEditor::DestroyEditor(m_Context);
        }

        void EditorWindowNodeEditor::DrawInternal()
        {
            auto& io = ImGui::GetIO();

            ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

            ImGui::Separator();

            ax::NodeEditor::SetCurrentEditor(m_Context);
            ax::NodeEditor::Begin("My Editor", ImVec2(0.0, 0.0f));
            int uniqueId = 1;

            // Start drawing nodes.
            ax::NodeEditor::BeginNode(uniqueId++);
            ImGui::Text("Node A");
            ax::NodeEditor::BeginPin(uniqueId++, ax::NodeEditor::PinKind::Input);
            ImGui::Text("-> In");
            ax::NodeEditor::EndPin();
            ImGui::SameLine();
            ax::NodeEditor::BeginPin(uniqueId++, ax::NodeEditor::PinKind::Output);
            ImGui::Text("Out ->");
            ax::NodeEditor::EndPin();
            ax::NodeEditor::EndNode();
            ax::NodeEditor::End();
            ax::NodeEditor::SetCurrentEditor(nullptr);
        }

	}

}