#pragma once

#include "QE_EditorWindow.h"

// #TODO Remove includes from header
#ifdef _QIMGUINODEEDITOR
#include "Libraries/imgui/imgui.h"
#include "Libraries/imgui/imgui_internal.h"
#include "Libraries/imgui-node-editor/imgui_node_editor.h"
namespace ed = ax::NodeEditor;
#include "Libraries/imgui-node-editor/utilities/widgets.h"
#endif

namespace QwerkE {

	namespace Editor {

		class EditorWindowNodeEditor : public EditorWindow
		{
		public:
            EditorWindowNodeEditor(GUID guid = GUID());
            ~EditorWindowNodeEditor();

            struct BlueprintEditor
            {
                int NextPinId = 1;
                int NextLinkId = 1;

                enum class PinType { Flow, Bool, Int, Float };

                ImColor GetPinColor(PinType type)
                {
                    switch (type)
                    {
                    case PinType::Flow:  return ImColor(255, 255, 255);
                    case PinType::Bool:  return ImColor(220, 48, 48);
                    case PinType::Int:   return ImColor(68, 201, 156);
                    case PinType::Float: return ImColor(147, 226, 74);
                    }
                    return ImColor(255, 255, 255);
                }

                template <typename T>
                struct TemplatedPin
                {
                    // const Mirror::TypeInfo* TypeInfo() { return Mirror::InfoForType<T>(); }
                };

                struct Pin
                {
                    ed::PinId Id;
                    std::string Name;
                    PinType Type;
                };

                class Node
                {
                public:
                    Node(BlueprintEditor& bpEditor) { }

                    virtual void Execute() {};
                    virtual void Draw()
                    {
                        ed::BeginNode(Id);
                        ImGui::BeginGroup();
                        ImGui::Dummy({ Size.x, 0.f });
                        // ed::SetGroupSize(Id, { Size.x, Size.y });
                        // ImGui::SetNextItemWidth(Size.x);
                        ImGui::Text("%s", Name.c_str());
                        // ImGui::Dummy({ Size.x, Size.y });
                        ImGui::EndGroup();

                        const bool isConnected = false;
                        for (size_t i = 0; i < Inputs.size(); i++)
                        {
                            Pin pin = Inputs[i];
                            ed::BeginPin(pin.Id, ed::PinKind::Input);
                            ax::Widgets::Icon(ImVec2(static_cast<float>(24), static_cast<float>(24)), ax::Widgets::IconType::Circle, isConnected, ImColor(255, 255, 255, 255), ImColor(32, 32, 32, 32));
                            ed::EndPin();
                        }

                        for (size_t i = 0; i < Outputs.size(); i++)
                        {
                            Pin pin = Outputs[i];
                            ed::BeginPin(pin.Id, ed::PinKind::Output);
                            ax::Widgets::Icon(ImVec2(static_cast<float>(24), static_cast<float>(24)), ax::Widgets::IconType::Circle, isConnected, ImColor(255, 255, 255, 255), ImColor(32, 32, 32, 32));
                            ed::EndPin();
                        }

                        ed::EndNode();
                    };

                    virtual void OnConnect() {};
                    virtual void OnDisconnect() {};

                    ed::NodeId Id;
                    std::string Name;
                    std::vector<Pin> Inputs;
                    std::vector<Pin> Outputs;

                    ImVec2 Size; // Effectively a minimum size as larger elements will force increased node size
                    // ImColor Color;
                    ImU32 Color; // #TODO Review ImU32 coloring

                private:
                    bool isEnabled = true;
                    char attributes = '0';
                };

                struct Link
                {
                    ed::LinkId LinkId;
                    ed::PinId StartPinId;
                    ed::PinId EndPinId;
                };

                struct Comment
                {
                    ed::NodeId Id;
                    ImVec2     Size;
                    std::string Text;
                    // ImColor    Color;
                    ImU32    Color; // #TODO Review ImU32 coloring
                };

                std::vector<Node*> Nodes;
                std::vector<Link> Links;
                std::vector<Comment> Comments;
            };

		private:
			void DrawInternal() override;

            BlueprintEditor m_BpEditor;
			void CreateBluePrintsExample();
			void DrawBluePrintsExample();
            void DestroyBluePrintsExample();

            ax::NodeEditor::EditorContext* m_Context = nullptr;
            const char* m_SettingsFileName = "NodeEditorWidgets.json";
			s32 m_NodeExample = 2;
		};

	}

}