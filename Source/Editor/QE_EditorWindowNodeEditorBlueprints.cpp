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

#include "QF_Color.h"
#include "QF_Colors.h"
#include "QF_ColorPalettes.h"

#include "QF_Time.h" // #TODO Remove test include

namespace QwerkE {

	namespace Editor {

		class NodeBranch : public EditorWindowNodeEditor::BlueprintEditor::Node
		{
		public:
			NodeBranch(EditorWindowNodeEditor::BlueprintEditor& bpEditor) :
				EditorWindowNodeEditor::BlueprintEditor::Node(bpEditor),
				m_BpEditor(&bpEditor)
			{
				pinIn.Id = bpEditor.NextPinId++;
				pinOut1.Id = bpEditor.NextPinId++;
				pinOut2.Id = bpEditor.NextPinId++;
			}

			void Execute() override
			{
				if (m_BpEditor)
				{
					for (size_t i = 0; i < m_BpEditor->Links.size(); i++)
					{
						const EditorWindowNodeEditor::BlueprintEditor::Link& link = m_BpEditor->Links[i];
						if (link.EndPinId == pinIn.Id)
						{
							int a = 0;
							LOG_TRACE("{0} Connected", __FUNCTION__);
						}
					}
				}

				bool inputConnectionId = true; // pinIn.Id.hasConnection // #TODO Evaluate input pin connection
				if (inputConnectionId)
				{
					bool inputConnectionCondition = true; // #TODO Evaluate input pin condition
					if (inputConnectionCondition)
					{
						// if (pinOut1.hasConnection)
						{
							// #TODO Get pinOut1 connection and execute it
						}
					}
					else
					{
						// if (pinOut2.hasConnection)
						{
							// #TODO Get pinOut2 connection and execute it
						}
					}
				}
			};

			void Draw() override
			{
				bool isConnected = false; // #TODO Get connected pin state
				float _width = 100.f;

				ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(Color));
				ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(10, 10, 10, 255));
				ed::PushStyleColor(ed::StyleColor_PinRect, ImColor(60, 60, 60, 150));
				ed::PushStyleColor(ed::StyleColor_PinRectBorder, ImColor(100, 100, 100, 150));

				ed::BeginNode(Id);
				ImGui::Text("%s", Name.c_str());

				ImGui::BeginGroup();
				ImGui::Dummy({ Size.x, 0 });

				ed::BeginPin(pinIn.Id, ed::PinKind::Input);
				ax::Widgets::Icon(ImVec2(static_cast<float>(24), static_cast<float>(24)), ax::Widgets::IconType::Circle, isConnected, ImColor(255, 255, 255, 255), ImColor(32, 32, 32, 32));
				ed::EndPin();

				ImGui::SameLine();
				ImGui::Dummy({ 25.f, 0.f });
				ImGui::SameLine();
				ed::BeginPin(2000, ed::PinKind::Output);
				ax::Widgets::Icon(ImVec2(static_cast<float>(24), static_cast<float>(24)), ax::Widgets::IconType::Circle, isConnected, ImColor(255, 255, 255, 255), ImColor(32, 32, 32, 32));
				ed::EndPin();

				ImGui::Dummy({ 57.f, 0.f });
				ImGui::SameLine();
				ed::BeginPin(pinOut2.Id, ed::PinKind::Output);
				ax::Widgets::Icon(ImVec2(static_cast<float>(24), static_cast<float>(24)), ax::Widgets::IconType::Circle, isConnected, ImColor(255, 255, 255, 255), ImColor(32, 32, 32, 32));
				ed::EndPin();

				ImGui::EndGroup();

				ed::EndNode();

				ed::PopStyleColor(4);
			};

			void OnConnect() override {};
			void OnDisconnect() override {};

			EditorWindowNodeEditor::BlueprintEditor::Pin pinIn;
			EditorWindowNodeEditor::BlueprintEditor::Pin pinOut1;
			EditorWindowNodeEditor::BlueprintEditor::Pin pinOut2;

		private:
			EditorWindowNodeEditor::BlueprintEditor* m_BpEditor = nullptr;
		};

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

				{	// Empty base node
					BlueprintEditor::Node* node = new BlueprintEditor::Node(m_BpEditor);
					node->Id = m_BpEditor.NextPinId++;
					node->Name = "Test Node";
					node->Size = ImVec2(100, 80);
					node->Color = ImColor(150, 150, 255, 64);
					node->Inputs.push_back({ static_cast<ax::NodeEditor::PinId>(m_BpEditor.NextPinId++), "In", BlueprintEditor::PinType::Flow });
					node->Outputs.push_back({ static_cast<ax::NodeEditor::PinId>(m_BpEditor.NextPinId++), "Out", BlueprintEditor::PinType::Flow });
					m_BpEditor.Nodes.push_back(node);
				}

				{	// Empty base node
					BlueprintEditor::Node* node = new BlueprintEditor::Node(m_BpEditor);
					node->Id = m_BpEditor.NextPinId++;
					node->Name = "Test Node2";
					node->Size = ImVec2(150, 80);
					node->Color = ImColor(150, 150, 255, 64);
					node->Inputs.push_back({ static_cast<ax::NodeEditor::PinId>(m_BpEditor.NextPinId++), "In", BlueprintEditor::PinType::Flow });
					node->Outputs.push_back({ static_cast<ax::NodeEditor::PinId>(m_BpEditor.NextPinId++), "Out", BlueprintEditor::PinType::Flow });
					m_BpEditor.Nodes.push_back(node);
				}

				{	// Example branch node
					NodeBranch* node = new NodeBranch(m_BpEditor);
					node->Id = m_BpEditor.NextPinId++;
					node->Name = "Branch Node";
					node->Size = ImVec2(150, 80);
					node->Color = ImColor(150, 150, 255, 64);
					m_BpEditor.Nodes.push_back(node);
				}

				{	// Example comment node
					BlueprintEditor::Comment comment;
					comment.Id = m_BpEditor.NextPinId++;
					comment.Size = ImVec2(300, 200);
					comment.Text = "My Comment";
					// comment.Color = ImColor(255, 255, 0, 64);
					static int rollingInt = 0;
					Color4 paletteColor = Colors::Palettes::Default(rollingInt++);
					comment.Color = ImColor(paletteColor.r, paletteColor.g, paletteColor.b, paletteColor.a);
					m_BpEditor.Comments.push_back(comment);
				}

				BlueprintEditor_ApplyStyle();
				BlueprintEditor_ApplyColours();

				ed::SetCurrentEditor(nullptr);
			}
		}

		void EditorWindowNodeEditor::DrawBluePrintsExample()
		{
			ed::SetCurrentEditor(m_Context);

			ed::Begin("Node Editor");

			{	// Slider float sample node showing color lerping
				static float sliderFloat = 0.5f;
				Color4 color4 = Colors::Lerp({ 0.f, 1.f, 0.f, 1.f }, { 1.f, 0.f, 0.f, 1.f }, sliderFloat);
				ImVec4 imColor4{ color4.r, color4.g, color4.b, color4.a };

				ImGui::PushStyleColor(ImGuiCol_Button, imColor4);
				ed::BeginNode(0x10000);
				if (ImGui::SliderFloat("", &sliderFloat, 0.f, 1.f))
				{
				}
				if (ImGui::Button("Slider##SliderFloatButton"))
				{
				}
				ImGui::PopStyleColor(1);
				if (ImGui::Button("Slider##SliderFloatButton2"))
				{
				}
				ed::EndNode();
			}

			//// Execute
			// Execute nodes
			for (auto& node : m_BpEditor.Nodes)
			{
				node->Execute();
			}

			//// Draw
			// Draw comments
			ed::PushStyleVar(ed::StyleVar_NodePadding, ImVec4(8, 8, 8, 8));
			for (auto& comment : m_BpEditor.Comments)
			{
				static float timer = 0.0f;
				timer += 0.1f * Time::PreviousFrameDuration();
				if (timer > 1.f)
				{
					timer = 0.f;
				}

				std::array<Colors::ColorStop, 5> HeatmapStops = { {
					{0.0f,  Color4(0.f, 0.f, 1.f, 1.f)}, // Blue
					{0.25f, Color4(0.f, 1.f, 1.f, 1.f)}, // Cyan
					{0.5f,  Color4(0.f, 1.f, 0.f, 1.f)}, // Green
					{0.75f, Color4(1.f, 1.f, 0.f, 1.f)}, // Yellow
					{1.0f,  Color4(1.f, 0.f, 0.f, 1.f)}  // Red
				} };
				Color4 c = Gradient(timer, HeatmapStops);
				// Color4 c = Colors::Spectrum(timer);
				ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(c.r, c.g, c.b, c.a));

				ed::BeginNode(comment.Id);
				// ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImU32)ImColor(c.r, c.g, c.b, c.a));
				// ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImU32)ImColor(c.r * 255.f, c.g * 255.f, c.b * 255.f, c.a * 255.f));
				// ImGui::PushStyleColor(ImGuiCol_ChildBg, comment.Color);

				ImGui::BeginGroup();
				ImGui::TextUnformatted(comment.Text.c_str());
				ImGui::Dummy(comment.Size);
				ImGui::EndGroup();

				// ImGui::PopStyleColor();

				ed::EndNode();
				ed::PopStyleColor(1);
				ed::SetNodeZPosition(comment.Id, -1); // push behind nodes
			}
			ed::PopStyleVar();

			// Draw nodes
			for (auto& node : m_BpEditor.Nodes)
			{
				node->Draw();
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
					comment.Id = m_BpEditor.NextPinId++;
					comment.Size = ImVec2(300, 200);
					comment.Text = "New Comment";
					comment.Color = ImColor(255, 255, 0, 64);

					m_BpEditor.Comments.push_back(comment);
				}
				if (ImGui::MenuItem("Add Branch"))
				{
					NodeBranch* branch = new NodeBranch(m_BpEditor);
					branch->Id = m_BpEditor.NextPinId++;
					branch->Size = ImVec2(300, 200);
					branch->Name = "Branch";
					branch->Size = ImVec2(100, 80);

					static int rollingInt = 0; // #TODO Improve color cycle test code
					const float colorRange = 15.f;
					Color4 paletteColor = Colors::Spectrum(rollingInt++ % (u32)colorRange / colorRange);
					branch->Color = ImColor(paletteColor.r, paletteColor.g, paletteColor.b, paletteColor.a);

					branch->pinIn.Id = m_BpEditor.NextPinId++;
					branch->pinOut1.Id = m_BpEditor.NextPinId++;
					branch->pinOut2.Id = m_BpEditor.NextPinId++;

					m_BpEditor.Nodes.push_back(branch);
				}
				ImGui::EndPopup();
			}
			ed::Resume();

			// Draw links
			for (auto& link : m_BpEditor.Links)
			{
				if (false) // #TODO Why again?
				{
					ed::Flow(link.LinkId);
				}
				ImColor linkColor(255, 180, 10, 255);
				const float thickness = 1.f;
				ed::Link(link.LinkId, link.StartPinId, link.EndPinId, linkColor, thickness);
			}

			// ==================================================================================================
			// Interaction Handling Section
			// This was copied from BasicInteration.cpp. See that file for commented code.

			// Handle creation action ---------------------------------------------------------------------------
			if (ed::BeginCreate())
			{
				ed::PinId inputPinId, outputPinId;
				if (ed::QueryNewLink(&inputPinId, &outputPinId))
				{
					if (inputPinId && outputPinId)
					{
						if (ed::AcceptNewItem())
						{
							m_BpEditor.Links.push_back({ ed::LinkId(m_BpEditor.NextPinId++), inputPinId, outputPinId });
							ed::Link(m_BpEditor.Links.back().LinkId, m_BpEditor.Links.back().StartPinId, m_BpEditor.Links.back().EndPinId);
						}
					}
				}
			}
			ed::EndCreate();

			// Handle deletion action ---------------------------------------------------------------------------
			if (ed::BeginDelete())
			{
				ed::LinkId deletedLinkId;
				while (ed::QueryDeletedLink(&deletedLinkId))
				{
					if (ed::AcceptDeletedItem())
					{
						for (size_t i = 0; i < m_BpEditor.Links.size(); i++)
						{
							EditorWindowNodeEditor::BlueprintEditor::Link link = m_BpEditor.Links[i];
							if (link.LinkId == deletedLinkId)
							{
								m_BpEditor.Links.erase(m_BpEditor.Links.begin() + i);
								break;
							}
						}
					}
				}
			}
			ed::EndDelete();

			ed::End();
			ed::SetCurrentEditor(nullptr);
		}

		void EditorWindowNodeEditor::DestroyBluePrintsExample()
		{
			for (auto node : m_BpEditor.Nodes)
			{
				delete node;
			}
			m_BpEditor.Nodes.clear();
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
