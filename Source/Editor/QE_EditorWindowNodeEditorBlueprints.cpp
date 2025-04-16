#include "QE_EditorWindowNodeEditor.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>

#define IMUI_EXTENDED_WIP
#ifdef IMUI_EXTENDED_WIP
#ifdef _QIMGUINODEEDITOR
#include "Libraries/imgui/imgui_internal.h"
#include "Libraries/imgui-node-editor/imgui_node_editor.h"
namespace ed = ax::NodeEditor;
#include "Libraries/imgui-node-editor/utilities/builders.h"
#include "Libraries/imgui-node-editor/utilities/widgets.h"
#endif
#endif

namespace QwerkE {

	namespace Editor {

        void EditorWindowNodeEditor::DrawBluePrintsExample()
        {
            ed::SetCurrentEditor(m_Context);
            ed::Begin("My Editor", ImVec2(0.0, 0.0f));
            int uniqueId = 1;
            // Start drawing nodes.
            ed::BeginNode(uniqueId++);
            ImGui::Text("Node A");
            ed::BeginPin(uniqueId++, ed::PinKind::Input);
            ImGui::Text("-> In");
            ed::EndPin();
            ImGui::SameLine();
            ed::BeginPin(uniqueId++, ed::PinKind::Output);
            ImGui::Text("Out ->");
            ed::EndPin();
            ed::EndNode();
            ed::End();
            ed::SetCurrentEditor(nullptr);
        }

	}

}