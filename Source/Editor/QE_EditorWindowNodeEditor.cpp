#include "QE_EditorWindowNodeEditor.h"

#undef _QIMGUINODEEDITOR
#ifdef _QIMGUINODEEDITOR
#include "Libraries/imgui/imgui.h"
#include "Libraries/imgui/imgui_internal.h"
#include "Libraries/imgui-node-editor/imgui_node_editor.h"

# ifdef _MSC_VER
# define portable_strcpy    strcpy_s
# define portable_sprintf   sprintf_s
# else
# define portable_strcpy    strcpy
# define portable_sprintf   sprintf
# endif
namespace ed = ax::NodeEditor;
#endif

// #define _QIMNODEFLOW
#ifdef _QIMNODEFLOW
#include "Libraries/imgui/imgui.h"
#include "Libraries/ImNodeFlow/ImNodeFlow.h"
#endif // _QIMNODEFLOW

#ifdef _QIMNODEFLOW
// ImNodeFlow start
using namespace ImFlow;

class SimpleSum : public BaseNode
{

public:
    SimpleSum()
    {
        setTitle("Simple sum");
        setStyle(NodeStyle::green());
        BaseNode::addIN<int>("In", 0, ConnectionFilter::SameType());
        BaseNode::addOUT<int>("Out", nullptr)->behaviour([this]() { return getInVal<int>("In") + m_valB; });
    }

    void draw() override
    {
        if (BaseNode::isSelected()) {
            ImGui::SetNextItemWidth(100.f);
            ImGui::InputInt("##ValB", &m_valB);
            ImGui::Button("Hello");
        }
    }

private:
    int m_valB = 0;
};

struct NodeEditor : ImFlow::BaseNode
{
    ImFlow::ImNodeFlow mINF;
    NodeEditor(float d, std::size_t r)
        : BaseNode()
    {
        setTitle("glhf");
        mINF.setSize({ d,d });
        if (r > 0) {
            mINF.addNode<SimpleSum>({ 0,0 });
            mINF.addNode<SimpleSum>({ 10,10 });
        }
    }

    void set_size(ImVec2 d)
    {
        mINF.setSize(d);
    }

    void draw() override
    {
        mINF.update();
    }
};

NodeEditor neditor(500, 1500);
// ImNodeFlow end
#endif // _QIMNODEFLOW

namespace QwerkE {

	namespace Editor {

#ifdef _QIMGUINODEEDITOR
        struct LinkInfo
        {
            ed::LinkId Id;
            ed::PinId  InputId;
            ed::PinId  OutputId;
        };

        // #TODO Avoid static
        static ImVector<LinkInfo> m_Links; // List of live links. It is dynamic unless you want to create read-only view over nodes.
        static int m_NextLinkId = 100; // Counter to help generate link ids. In real application this will probably based on pointer to user data structure.
#endif // _QIMGUINODEEDITOR

        EditorWindowNodeEditor::EditorWindowNodeEditor(GUID guid) :
            EditorWindow("Node Editor", EditorWindowTypes::NodeEditor, guid)
        {
#ifdef _QIMGUINODEEDITOR
            ax::NodeEditor::Config config;
            config.SettingsFile = m_SettingsFileName;
            m_Context = ax::NodeEditor::CreateEditor(&config);
#endif // _QIMGUINODEEDITOR
        }

        EditorWindowNodeEditor::~EditorWindowNodeEditor()
        {
#ifdef _QIMGUINODEEDITOR
            ax::NodeEditor::DestroyEditor(m_Context);
#endif // _QIMGUINODEEDITOR
        }

        void local_ImNodeFlow();
        void local_SimpleNodeEditor(ax::NodeEditor::EditorContext* context, const std::string& nodeEditorName);
        void local_WidgetsNodeEditor(ax::NodeEditor::EditorContext* context);

        void EditorWindowNodeEditor::DrawInternal()
        {
            char buffer[5];
            if (ImGui::Combo("Node Editor Style", &m_NodeExample,
                    "Simple\0"
                    "Widget\0"
                    "BluePrint\0"
                    "ImNodeFlow\0"
                ))
            {
            }
            switch (m_NodeExample)
            {
            case 0:
                local_SimpleNodeEditor(m_Context, m_WindowName);
                break;
            case 1:
                // #TODO Fix deallocation crash
                local_WidgetsNodeEditor(m_Context);
                break;
            case 2:
                DrawBluePrintsExample();
                break;
            case 3:
                local_ImNodeFlow();
                break;
            }
        }

        void local_ImNodeFlow()
        {
            if (true) // #TODO bx de-allocation null pointer exception
            {
#ifdef _QIMNODEFLOW
                // ImGuiIO& io = ImGui::GetIO(); // (void)io;?
                // const ImVec2 window_size = io.DisplaySize - ImVec2(1, 1);
                // const ImVec2 window_pos = ImVec2(1, 1);
                // const ImVec2 node_editor_size = window_size - ImVec2(16, 16);
                // neditor.set_size(node_editor_size);

                // ImGui::Begin("Node Editor", nullptr, 0); // ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
                neditor.set_size(ImGui::GetContentRegionAvail());
                neditor.draw();
                // ImGui::End();
#endif // _QIMNODEFLOW
            }
        }

        void local_SimpleNodeEditor(ax::NodeEditor::EditorContext* context, const std::string& nodeEditorName)
        {
#ifdef _QIMGUINODEEDITOR
            auto& io = ImGui::GetIO();

            ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

            ImGui::Separator();

            ax::NodeEditor::SetCurrentEditor(context);
            ax::NodeEditor::Begin(nodeEditorName.c_str(), ImVec2(0.0, 0.0f));
            int uniqueId = 1;

            for (size_t i = 0; i < 3; i++)
            {
                std::string nodeName = "Node ";
                nodeName += (char)(65 + i);

                ax::NodeEditor::BeginNode(uniqueId++);
                ImGui::Text(nodeName.c_str());
                ax::NodeEditor::BeginPin(uniqueId++, ax::NodeEditor::PinKind::Input);
                ImGui::Text("-> In");
                ax::NodeEditor::EndPin();
                ImGui::SameLine();
                ax::NodeEditor::BeginPin(uniqueId++, ax::NodeEditor::PinKind::Output);
                ImGui::Text("Out ->");
                ax::NodeEditor::EndPin();
                ax::NodeEditor::EndNode();
            }

            ax::NodeEditor::End();
            ax::NodeEditor::SetCurrentEditor(nullptr);
#endif // _QIMGUINODEEDITOR
        }

        void local_WidgetsNodeEditor(ax::NodeEditor::EditorContext* context)
        {
#ifdef _QIMGUINODEEDITOR
            static bool firstframe = true; // Used to position the nodes on startup
            auto& io = ImGui::GetIO();

            // FPS Counter Ribbon
            ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
            // ImGui::Separator();

            // Node Editor Widget
            ed::SetCurrentEditor(context);
            ed::Begin("My Editor", ImVec2(0.0, 0.0f));
            int uniqueId = 1;

            // Basic Widgets Demo  ==============================================================================================
            auto basic_id = uniqueId++;
            ed::BeginNode(basic_id);
            ImGui::Text("Basic Widget Demo");
            ed::BeginPin(uniqueId++, ed::PinKind::Input);
            ImGui::Text("-> In");
            ed::EndPin();
            ImGui::SameLine();
            ImGui::Dummy(ImVec2(250, 0)); // Hacky magic number to space out the output pin.
            ImGui::SameLine();
            ed::BeginPin(uniqueId++, ed::PinKind::Output);
            ImGui::Text("Out ->");
            ed::EndPin();

            // Widget Demo from imgui_demo.cpp...
            // Normal Button
            static int clicked = 0;
            if (ImGui::Button("Button"))
                clicked++;
            if (clicked & 1)
            {
                ImGui::SameLine();
                ImGui::Text("Thanks for clicking me!");
            }

            // Checkbox
            static bool check = true;
            ImGui::Checkbox("checkbox", &check);

            // Radio buttons
            static int e = 0;
            ImGui::RadioButton("radio a", &e, 0); ImGui::SameLine();
            ImGui::RadioButton("radio b", &e, 1); ImGui::SameLine();
            ImGui::RadioButton("radio c", &e, 2);

            // Color buttons, demonstrate using PushID() to add unique identifier in the ID stack, and changing style.
            for (int i = 0; i < 7; i++)
            {
                if (i > 0)
                    ImGui::SameLine();
                ImGui::PushID(i);
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
                ImGui::Button("Click");
                ImGui::PopStyleColor(3);
                ImGui::PopID();
            }

            // Use AlignTextToFramePadding() to align text baseline to the baseline of framed elements (otherwise a Text+SameLine+Button sequence will have the text a little too high by default)
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Hold to repeat:");
            ImGui::SameLine();

            // Arrow buttons with Repeater
            static int counter = 0;
            float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
            ImGui::PushButtonRepeat(true);
            if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { counter--; }
            ImGui::SameLine(0.0f, spacing);
            if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { counter++; }
            ImGui::PopButtonRepeat();
            ImGui::SameLine();
            ImGui::Text("%d", counter);

            // The input widgets also require you to manually disable the editor shortcuts so the view doesn't fly around.
            // (note that this is a per-frame setting, so it disables it for all text boxes.  I left it here so you could find it!)
            ed::EnableShortcuts(!io.WantTextInput);
            // The input widgets require some guidance on their widths, or else they're very large. (note matching pop at the end).
            ImGui::PushItemWidth(200);
            static char str1[128] = "";
            ImGui::InputTextWithHint("input text (w/ hint)", "enter text here", str1, IM_ARRAYSIZE(str1));

            static float f0 = 0.001f;
            ImGui::InputFloat("input float", &f0, 0.01f, 1.0f, "%.3f");

            static float f1 = 1.00f, f2 = 0.0067f;
            ImGui::DragFloat("drag float", &f1, 0.005f);
            ImGui::DragFloat("drag small float", &f2, 0.0001f, 0.0f, 0.0f, "%.06f ns");
            ImGui::PopItemWidth();

            ed::EndNode();
            if (firstframe)
            {
                ed::SetNodePosition(basic_id, ImVec2(20, 20));
            }

            // Headers and Trees Demo =======================================================================================================
            // TreeNodes and Headers streatch to the entire remaining work area. To put them in nodes what we need to do is to tell
            // ImGui out work area is shorter. We can achieve that right now only by using columns API.
            //
            // Relevent bugs: https://github.com/thedmd/imgui-node-editor/issues/30
            auto header_id = uniqueId++;
            ed::BeginNode(header_id);
            ImGui::Text("Tree Widget Demo");

            // Pins Row
            ed::BeginPin(uniqueId++, ed::PinKind::Input);
            ImGui::Text("-> In");
            ed::EndPin();
            ImGui::SameLine();
            ImGui::Dummy(ImVec2(35, 0)); //  magic number - Crude & simple way to nudge over the output pin. Consider using layout and springs
            ImGui::SameLine();
            ed::BeginPin(uniqueId++, ed::PinKind::Output);
            ImGui::Text("Out ->");
            ed::EndPin();

            // Tree column startup -------------------------------------------------------------------
            // Push dummy widget to extend node size. Columns do not do that.
            float width = 135; // bad magic numbers. used to define width of tree widget
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
            ImGui::Dummy(ImVec2(width, 0));
            ImGui::PopStyleVar();

            // Start columns, but use only first one.
            ImGui::BeginColumns("##TreeColumns", 2,
                ImGuiOldColumnFlags_NoBorder |
                ImGuiOldColumnFlags_NoResize |
                ImGuiOldColumnFlags_NoPreserveWidths |
                ImGuiOldColumnFlags_NoForceWithinWindow);

            // Adjust column width to match requested one.
            ImGui::SetColumnWidth(0, width
                + ImGui::GetStyle().WindowPadding.x
                + ImGui::GetStyle().ItemSpacing.x);
            // End of tree column startup --------------------------------------------------------------

            // Back to normal ImGui drawing, in our column.
            if (ImGui::CollapsingHeader("Open Header"))
            {
                ImGui::Text("Hello There");
                if (ImGui::TreeNode("Open Tree")) {
                    static bool OP1_Bool = false;
                    ImGui::Text("Checked: %s", OP1_Bool ? "true" : "false");
                    ImGui::Checkbox("Option 1", &OP1_Bool);
                    ImGui::TreePop();
                }
            }
            // Tree Column Shutdown
            ImGui::EndColumns();
            ed::EndNode(); // End of Tree Node Demo

            if (firstframe)
            {
                ed::SetNodePosition(header_id, ImVec2(420, 20));
            }

            // Tool Tip & Pop-up Demo =====================================================================================
            // Tooltips, combo-boxes, drop-down menus need to use a work-around to place the "overlay window" in the canvas.
            // To do this, we must defer the popup calls until after we're done drawing the node material.
            //
            // Relevent bugs:  https://github.com/thedmd/imgui-node-editor/issues/48
            auto popup_id = uniqueId++;
            ed::BeginNode(popup_id);
            ImGui::Text("Tool Tip & Pop-up Demo");
            ed::BeginPin(uniqueId++, ed::PinKind::Input);
            ImGui::Text("-> In");
            ed::EndPin();
            ImGui::SameLine();
            ImGui::Dummy(ImVec2(85, 0)); // Hacky magic number to space out the output pin.
            ImGui::SameLine();
            ed::BeginPin(uniqueId++, ed::PinKind::Output);
            ImGui::Text("Out ->");
            ed::EndPin();

            // Tooltip example
            ImGui::Text("Hover over me");
            static bool do_tooltip = false;
            do_tooltip = ImGui::IsItemHovered() ? true : false;
            ImGui::SameLine();
            ImGui::Text("- or me");
            static bool do_adv_tooltip = false;
            do_adv_tooltip = ImGui::IsItemHovered() ? true : false;

            // Use AlignTextToFramePadding() to align text baseline to the baseline of framed elements
            // (otherwise a Text+SameLine+Button sequence will have the text a little too high by default)
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Option:");
            ImGui::SameLine();
            static char popup_text[128] = "Pick one!";
            static bool do_popup = false;
            if (ImGui::Button(popup_text)) {
                do_popup = true;	// Instead of saying OpenPopup() here, we set this bool, which is used later in the Deferred Pop-up Section
            }
            ed::EndNode();
            if (firstframe) {
                ed::SetNodePosition(popup_id, ImVec2(610, 20));
            }

            // --------------------------------------------------------------------------------------------------
            // Deferred Pop-up Section

            // This entire section needs to be bounded by Suspend/Resume!  These calls pop us out of "node canvas coordinates"
            // and draw the popups in a reasonable screen location.
            ed::Suspend();
            // There is some stately stuff happening here.  You call "open popup" exactly once, and this
            // causes it to stick open for many frames until the user makes a selection in the popup, or clicks off to dismiss.
            // More importantly, this is done inside Suspend(), so it loads the popup with the correct screen coordinates!
            if (do_popup) {
                ImGui::OpenPopup("popup_button"); // Cause openpopup to stick open.
                do_popup = false; // disable bool so that if we click off the popup, it doesn't open the next frame.
            }

            // This is the actual popup Gui drawing section.
            if (ImGui::BeginPopup("popup_button")) {
                // Note: if it weren't for the child window, we would have to PushItemWidth() here to avoid a crash!
                ImGui::TextDisabled("Pick One:");
                ImGui::BeginChild("popup_scroller", ImVec2(100, 100), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
                if (ImGui::Button("Option 1")) {
                    portable_strcpy(popup_text, "Option 1");
                    ImGui::CloseCurrentPopup();  // These calls revoke the popup open state, which was set by OpenPopup above.
                }
                if (ImGui::Button("Option 2")) {
                    portable_strcpy(popup_text, "Option 2");
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::Button("Option 3")) {
                    portable_strcpy(popup_text, "Option 3");
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::Button("Option 4")) {
                    portable_strcpy(popup_text, "Option 4");
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndChild();
                ImGui::EndPopup(); // Note this does not do anything to the popup open/close state. It just terminates the content declaration.
            }

            // Handle the simple tooltip
            if (do_tooltip)
                ImGui::SetTooltip("I am a tooltip");

            // Handle the advanced tooltip
            if (do_adv_tooltip) {
                ImGui::BeginTooltip();
                ImGui::Text("I am a fancy tooltip");
                static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
                ImGui::PlotLines("Curve", arr, IM_ARRAYSIZE(arr));
                ImGui::EndTooltip();
            }

            ed::Resume();
            // End of "Deferred Pop-up section"

            // Plot Widgets =========================================================================================
            // Note: most of these plots can't be used in nodes missing, because they spawn tooltips automatically,
            // so we can't trap them in our deferred pop-up mechanism.  This causes them to fly into a random screen
            // location.
            auto plot_id = uniqueId++;
            ed::BeginNode(plot_id);
            ImGui::Text("Plot Demo");
            ed::BeginPin(uniqueId++, ed::PinKind::Input);
            ImGui::Text("-> In");
            ed::EndPin();
            ImGui::SameLine();
            ImGui::Dummy(ImVec2(250, 0)); // Hacky magic number to space out the output pin.
            ImGui::SameLine();
            ed::BeginPin(uniqueId++, ed::PinKind::Output);
            ImGui::Text("Out ->");
            ed::EndPin();

            ImGui::PushItemWidth(300);

            // Animate a simple progress bar
            static float progress = 0.0f, progress_dir = 1.0f;
            progress += progress_dir * 0.4f * ImGui::GetIO().DeltaTime;
            if (progress >= +1.1f) { progress = +1.1f; progress_dir *= -1.0f; }
            if (progress <= -0.1f) { progress = -0.1f; progress_dir *= -1.0f; }

            // Typically we would use ImVec2(-1.0f,0.0f) or ImVec2(-FLT_MIN,0.0f) to use all available width,
            // or ImVec2(width,0.0f) for a specified width. ImVec2(0.0f,0.0f) uses ItemWidth.
            ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("Progress Bar");

            float progress_saturated = (progress < 0.0f) ? 0.0f : (progress > 1.0f) ? 1.0f : progress;
            char buf[32];
            portable_sprintf(buf, "%d/%d", (int)(progress_saturated * 1753), 1753);
            ImGui::ProgressBar(progress, ImVec2(0.f, 0.f), buf);

            ImGui::PopItemWidth();
            ed::EndNode();
            if (firstframe) {
                ed::SetNodePosition(plot_id, ImVec2(850, 20));
            }
            // ==================================================================================================
            // Link Drawing Section

            for (auto& linkInfo : m_Links)
                ed::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);

            // ==================================================================================================
            // Interaction Handling Section
            // This was coppied from BasicInteration.cpp. See that file for commented code.

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
                            m_Links.push_back({ ed::LinkId(m_NextLinkId++), inputPinId, outputPinId });
                            ed::Link(m_Links.back().Id, m_Links.back().InputId, m_Links.back().OutputId);
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
                        for (auto& link : m_Links)
                        {
                            if (link.Id == deletedLinkId)
                            {
                                m_Links.erase(&link);
                                break;
                            }
                        }
                    }
                }
            }
            ed::EndDelete();

            ed::End();
            ed::SetCurrentEditor(nullptr);
            firstframe = false;
#endif // _QIMGUINODEEDITOR
        }

	}

}