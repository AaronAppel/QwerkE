#include "QE_EditorWindowNodeEditor.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>

// #ifdef IMUI_EXTENDED_WIP
#ifdef _QIMGUINODEEDITOR
#include "Libraries/imgui/imgui_internal.h"
#include "Libraries/imgui-node-editor/imgui_node_editor.h"
namespace ed = ax::NodeEditor;
#include "Libraries/imgui-node-editor/utilities/builders.h"
#include "Libraries/imgui-node-editor/utilities/widgets.h"

namespace QwerkE {

	namespace Editor {

		static bool s_Initialized = false;

		void BlueprintEditor_ApplyStyle()
		{
			auto& style = ed::GetStyle();

			style.NodePadding = ImVec4(8, 8, 8, 8);
			style.NodeRounding = 5.0f;
			style.NodeBorderWidth = 1.0f;

			style.HoveredNodeBorderWidth = 2.0f;
			style.SelectedNodeBorderWidth = 3.0f;

			style.PinRounding = 4.0f;
			style.PinBorderWidth = 1.0f;

			style.LinkStrength = 100.0f;

			// Flow animation
			style.FlowMarkerDistance = 30.0f;
			style.FlowSpeed = 150.0f;

			// Groups (used for comments)
			style.GroupRounding = 6.0f;
			style.GroupBorderWidth = 1.0f;
		}

		void BlueprintEditor_ApplyColours()
		{
			auto& colors = ed::GetStyle().Colors;

			colors[ed::StyleColor_NodeBg] = ImColor(40, 40, 50, 200);
			colors[ed::StyleColor_NodeBorder] = ImColor(100, 100, 120, 200);
			colors[ed::StyleColor_HovNodeBorder] = ImColor(150, 150, 180, 255);
			colors[ed::StyleColor_SelNodeBorder] = ImColor(255, 255, 255, 255);

			colors[ed::StyleColor_PinRect] = ImColor(60, 60, 70, 255);
			colors[ed::StyleColor_PinRectBorder] = ImColor(150, 150, 150, 255);

			// colors[ed::StyleColor_Link] = ImColor(200, 200, 100, 255);
			colors[ed::StyleColor_Flow] = ImColor(200, 200, 100, 255);
			// colors[ed::StyleColor_HovLink] = ImColor(255, 255, 150, 255);
			colors[ed::StyleColor_HovLinkBorder] = ImColor(255, 255, 150, 255);
			// colors[ed::StyleColor_SelLink] = ImColor(255, 255, 255, 255);
			colors[ed::StyleColor_SelLinkBorder] = ImColor(255, 255, 255, 255);

			// colors[ed::StyleColor_Background] = ImColor(30, 30, 40, 255);
			colors[ed::StyleColor_Bg] = ImColor(30, 30, 40, 255);
			colors[ed::StyleColor_Grid] = ImColor(50, 50, 60, 255);
		}

		void ApplyStyle2()
		{
			auto& style = ed::GetStyle();

			style.NodePadding = ImVec4(8, 8, 8, 8);
			style.NodeRounding = 5.0f;

			auto& colors = style.Colors;
			colors[ed::StyleColor_NodeBg] = ImColor(40, 40, 50, 200);
			// colors[ed::StyleColor_Link] = ImColor(200, 200, 100, 255);
			colors[ed::StyleColor_Grid] = ImColor(50, 50, 60, 255);
		}

		void EditorWindowNodeEditor::CreateBluePrintsExample()
		{
			// #TODO Fix Editor::NewEditorWindowByType() double construction
			// ASSERT(!s_Initialized, "Blueprints EditorContext* is already initialized!");

			if (!s_Initialized)
			{
				s_Initialized = true;

				BlueprintEditor::Node node;
				node.Id = m_BpEditor.NextId++;
				node.Name = "Test Node";
				m_BpEditor.Nodes.push_back(node);

				node.Inputs.push_back({ static_cast<ax::NodeEditor::PinId>(m_BpEditor.NextId++), "In", BlueprintEditor::PinType::Flow});
				node.Outputs.push_back({ static_cast<ax::NodeEditor::PinId>(m_BpEditor.NextId++), "Out", BlueprintEditor::PinType::Flow});

				BlueprintEditor::Comment comment;
				comment.Id = m_BpEditor.NextId++;
				comment.Size = ImVec2(300, 200);
				comment.Text = "My Comment";
				comment.Color = ImColor(255, 255, 0, 64);
				m_BpEditor.Comments.push_back(comment);

				BlueprintEditor_ApplyStyle();
				BlueprintEditor_ApplyColours();

				ed::SetCurrentEditor(nullptr);
			}
		}

		void EditorWindowNodeEditor::DrawBluePrintsExample()
		{
			ed::SetCurrentEditor(m_Context);

			ed::Begin("Node Editor");

			//// Draw
			// Draw comments
			ed::PushStyleVar(ed::StyleVar_NodePadding, ImVec4(8, 8, 8, 8));
			for (auto& comment : m_BpEditor.Comments)
			{
				ed::BeginNode(comment.Id);

				ImGui::PushStyleColor(ImGuiCol_ChildBg, comment.Color);

				ImGui::BeginGroup();
				ImGui::TextUnformatted(comment.Text.c_str());
				ImGui::Dummy(comment.Size);
				ImGui::EndGroup();

				ImGui::PopStyleColor();

				ed::EndNode();
				ed::SetNodeZPosition(comment.Id, -1); // push behind nodes
			}
			ed::PopStyleVar();

			// Draw nodes
			for (auto& node : m_BpEditor.Nodes)
			{
				ed::BeginNode(node.Id);

				ImGui::Text("%s", node.Name.c_str());

				ed::EndNode();
			}

			// Draw links
			for (auto& link : m_BpEditor.Links)
			{
				if (true)
				{
					ed::Flow(link.LinkId);
				}
				ed::Link(link.LinkId, link.StartPinId, link.EndPinId);
			}

			//// User interaction
			// Context menus, popups, UI
			ed::Suspend();
			if (ed::ShowBackgroundContextMenu())
			{
				ImGui::OpenPopup("NodeEditorContextMenu");
			}
			if (ImGui::BeginPopup("NodeEditorContextMenu"))
			{
				if (ImGui::MenuItem("Add Comment"))
				{
					BlueprintEditor::Comment comment;
					comment.Id = m_BpEditor.NextId++;
					comment.Size = ImVec2(300, 200);
					comment.Text = "New Comment";
					comment.Color = ImColor(255, 255, 0, 64);

					m_BpEditor.Comments.push_back(comment);
				}
				ImGui::EndPopup();
			}

			ed::Resume();

			ed::End();

			ed::SetCurrentEditor(nullptr);
		}

	}

}
#else
namespace QwerkE {

	namespace Editor {

		void EditorWindowNodeEditor::CreateBluePrintsExample(ax::NodeEditor::EditorContext* context) { }
		void EditorWindowNodeEditor::DrawBluePrintsExample() { }

	}

}
#endif
// #endif