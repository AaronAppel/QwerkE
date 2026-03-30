#pragma once

#include "QE_EditorWindow.h"

// #TODO Remove includes from header
#ifdef _QIMGUINODEEDITOR
#include "Libraries/imgui/imgui.h"
#include "Libraries/imgui/imgui_internal.h"
#include "Libraries/imgui-node-editor/imgui_node_editor.h"
namespace ed = ax::NodeEditor;
#endif

namespace QwerkE {

	namespace Editor {

		class EditorWindowNodeEditor : public EditorWindow
		{
		public:
            EditorWindowNodeEditor(GUID guid = GUID());
            ~EditorWindowNodeEditor();

		private:
			void DrawInternal() override;

            struct BlueprintEditor
            {
                int NextId = 1;

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

                struct Node
                {
                    ed::NodeId Id;
                    std::string Name;
                    std::vector<Pin> Inputs;
                    std::vector<Pin> Outputs;
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

                std::vector<Node> Nodes;
                std::vector<Link> Links;
                std::vector<Comment> Comments;
            };
            BlueprintEditor m_BpEditor;
			void CreateBluePrintsExample();
			void DrawBluePrintsExample();

            ax::NodeEditor::EditorContext* m_Context = nullptr;
            const char* m_SettingsFileName = "NodeEditorWidgets.json";
			s32 m_NodeExample = 2;
		};

	}

}