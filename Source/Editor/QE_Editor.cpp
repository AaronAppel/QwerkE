#include "QE_Editor.h"

#include <map>      // For std::map<std::string, const char*> pairs; and EditorWindows collection
#include <string>   // For std::map<std::string, const char*> pairs;
#include <typeinfo> // For typeid()
#include <vector>   // For s_EditorWindows

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <bx/timer.h>
#ifdef _QBGFXFRAMEWORK
#include <bgfxFramework/SampleRenderData.h>
#include <bgfxFramework/LoadShader.h>
#ifdef _QDEBUG
#include <bgfxFramework/debugDraw/debugdraw.h>
#endif // _QDEBUG
#endif // _QBGFXFRAMEWORK
#endif // _QBGFX

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QwerkE_imgui.h"
#include "Libraries/imgui_hex_editor/imgui_hex.h"
#include "Libraries/ImCoolBar/ImCoolBar.h"
#include "Libraries/ImFileDialog/ImFileDialog.h"
#include "Libraries/imgui_toggle/imgui_toggle.h"
#include "Libraries/imgui_toggle/imgui_toggle_presets.h"
#include "Libraries/imgui-knobs/imgui-knobs.h"
#include "Libraries/imgui-node-editor/imgui_node_editor.h"
#include "Libraries/imgui-notify/imgui_notify.h"
#include "Libraries/im-neo-sequencer/imgui_neo_sequencer.h"
#include "Libraries/ImGuizmo/ImGuizmo.h"
#include "Libraries/ImNodeFlow/ImNodeFlow.h"
#define IMSPINNER_DEMO
#include "Libraries/imspinner/imspinner.h"
#endif

#include "QC_Guid.h"
#include "QC_ProgramArgs.h"
#include "QC_System.h"
#include "QC_Time.h"

#include "QF_Assets.h"
#include "QF_Directory.h"
#include "QF_Files.h"
#include "QF_Framework.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Mesh.h"
#include "QF_PathDefines.h"
#include "QF_Paths.h"
#include "QF_Renderer.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Serialize.h"
#include "QF_Settings.h"
#include "QF_Window.h"

#include "QE_EditorWindowHelpers.h"
#include "QE_Projects.h"

static void imgui_toggle_example(); // Toggles
static void imgui_toggle_simple();
static void imgui_toggle_custom();
static void imgui_toggle_state(const ImGuiToggleConfig& config, ImGuiToggleStateConfig& state);

// From: https://gitlab.com/GroGy/im-neo-sequencer
int32_t currentFrame = 0; // Sequencer
int32_t startFrame = -10;
int32_t endFrame = 64;
bool transformOpen = false;
std::vector<ImGui::FrameIndexType> keys = { 0, 10, 24 };
bool doDelete = false;

// ImCoolBar
auto coolbar_button = [](const char* label) -> bool {
    float w = ImGui::GetCoolBarItemWidth();
    auto font_ptr = ImGui::GetIO().Fonts->Fonts[0];
    font_ptr->Scale = ImGui::GetCoolBarItemScale();
    ImGui::PushFont(font_ptr);
    bool res = ImGui::Button(label, ImVec2(w, w));
    ImGui::PopFont();
    return res;
};

// ImNodeFlow
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

namespace QwerkE {

    void LoadImGuiStyleFromFile() // #TODO Move somewhere else
    {
        ImGuiStyle& style = ImGui::GetStyle();
        Serialize::FromFile(Paths::Style(Settings::GetStyleFileName()).c_str(), style, true);
    }

	namespace Editor {

        static bool s_ShowingEditorUI = true;

        // #TODO Think of creating a WindowManager, or EditorWindows:: class to manage all window access, update, create/destroy,
        // and it can manage the stack of recently selected windows.
        static bool s_ShowingWindowStackPanel = false;
        static EditorWindow* s_WindowStackPanelLastSelected = nullptr;

        // #TODO Editor overlays(notifications), pop ups(cycle windows, load/save project), prompts(save unsaved change before quitting/closing scene/window)
        // #TODO Replace with ring buffer or better container to avoid "bubbling"/swapping all elements
        static std::vector<EditorWindow*> s_FocusedWindowsStack = std::vector<EditorWindow*>();

        static EditorWindowDockingContext s_EditorWindowDockingContext(GUID::Invalid); // #NOTE Draw order dependency with other EditorWindows

        constexpr char* s_EditorWindowDataFileName = "EditorWindowData.qdata";

        static EditorStateFlags s_EditorStateFlags = EditorStateFlags::EditorStateFlagsNone;

        void local_Initialize();
        void local_Shutdown();
        void local_Update();
        void local_EndFrame();

        bool local_StillRunning();
        void local_Stop();

        void local_FileDropCallback(const char* filePath);

        bool s_ReloadRequested = false; // #TODO FEATURE

        void DrawNotifyWindow();
        static bool temp = true;

        void RunReloadable(unsigned int numberOfArguments, char** commandLineArguments);

        void Run(unsigned int numberOfArguments, char** commandLineArguments)
        {
            do
            {
                if (s_ReloadRequested)
                {
                    Log::Console("\n-- Reloading Editor --\n\n");
                }
                s_ReloadRequested = false;
                RunReloadable(numberOfArguments, commandLineArguments);
            } while (s_ReloadRequested);
        }

		void RunReloadable(unsigned int numberOfArguments, char** commandLineArguments)
		{
            Log::Console("-- Qwerk Editor %s --\n", std::to_string(QWERKE_VERSION).c_str()); // #TODO Review QWERKE_VERSION as const char*

            Framework::SetCommandLineArgs(numberOfArguments, commandLineArguments); // #TODO Improve name

            Settings::LoadEngineSettings("Editor.qsetting");
            const EngineSettings& engineSettings = Settings::GetEngineSettings();
			Framework::Initialize(engineSettings.windowWidthPixels, engineSettings.windowHeightPixels);

            local_Initialize();

            // Hex Editor
            static ImGuiHexEditorState hex_state;
            ImColor user_highlight_color;

            const bool hexEditorEnabled = false; // #TODO Fix null pointer when imgui de-allocates
            if (hexEditorEnabled)
            {
                hex_state.ReadCallback = [](ImGuiHexEditorState* state, int offset, void* buf, int size) -> int {
                    SIZE_T read;
                    ReadProcessMemory(GetCurrentProcess(), (char*)state->Bytes + offset, buf, size, &read);
                    return read;
                    };

                hex_state.WriteCallback = [](ImGuiHexEditorState* state, int offset, void* buf, int size) -> int {
                    SIZE_T write;
                    WriteProcessMemory(GetCurrentProcess(), (char*)state->Bytes + offset, buf, size, &write);
                    return write;
                    };

                hex_state.GetAddressNameCallback = [](ImGuiHexEditorState* state, int offset, char* buf, int size) -> bool
                    {
                        if (offset >= 0 && offset < sizeof(ImGuiIO))
                        {
                            snprintf(buf, size, "io+%0.*zX", 4, offset);
                            return true;
                        }

                        return false;
                    };

                hex_state.SingleHighlightCallback = [](ImGuiHexEditorState* state, int offset, ImColor* color, ImColor* text_color, ImColor* border_color) -> ImGuiHexEditorHighlightFlags
                    {
                        if (offset >= 100 && offset <= 150)
                        {
                            *color = ImColor(); // Could not pass user_highlight_color into lambda by reference
                            return ImGuiHexEditorHighlightFlags_Apply | ImGuiHexEditorHighlightFlags_TextAutomaticContrast | ImGuiHexEditorHighlightFlags_Ascii
                                | ImGuiHexEditorHighlightFlags_BorderAutomaticContrast;
                        }

                        return ImGuiHexEditorHighlightFlags_None;
                    };

                hex_state.HighlightRanges.clear();

                {
                    ImGuiHexEditorHighlightRange range;
                    range.From = 200;
                    range.To = 250;
                    range.Color = ImColor(user_highlight_color);
                    range.Flags = ImGuiHexEditorHighlightFlags_TextAutomaticContrast | ImGuiHexEditorHighlightFlags_FullSized
                        | ImGuiHexEditorHighlightFlags_Ascii | ImGuiHexEditorHighlightFlags_Border | ImGuiHexEditorHighlightFlags_BorderAutomaticContrast;
                    hex_state.HighlightRanges.push_back(range);
                }

                hex_state.Bytes = (void*)&ImGui::GetIO();
                hex_state.MaxBytes = sizeof(ImGuiIO) + 0x1000;
            }

            Time::WriteAppStartTime();

            int spinInt = 0;
            float spinFloat = 0.f;
            double spinDouble = 0;

			while (local_StillRunning())
			{
				if (Time::ShouldProcessNextFrame())
				{
                    Time::StartFrame();

					Framework::StartFrame();

                    Renderer::StartImGui();

                    if (s_ShowingEditorUI && temp)
                    {
                        s_EditorWindowDockingContext.Draw();
                    }

                    local_Update();

					Framework::Update((float)Time::PreviousFrameDuration());

                    if (true)
                    {
                        if (ImGui::Begin("Spin Values"))
                        {
                            ImGui::PushItemWidth(150.f);
                            ImGui::ScrollerInt("Spin Int", &spinInt);
                            ImGui::ScrollerFloat("Spin Float", &spinFloat);
                            ImGui::ScrollerDouble("Spin Double", &spinDouble);
                            ImGui::PopItemWidth();
                        }
                        ImGui::End();
                    }

                    if (hexEditorEnabled)
                    {
                        if (ImGui::Begin("Hex Editor##Window"))
                        {
                            ImGui::BeginHexEditor("Hex Editor", &hex_state);
                            ImGui::EndHexEditor();
                        }
                        ImGui::End();
                    }

                    if (false)
                    {
                        ImGui::Begin("Knob knob");

                        static float val1 = 0;
                        if (ImGuiKnobs::Knob("Gain", &val1, -6.0f, 6.0f, 0.1f, "%.1fdB", ImGuiKnobVariant_Tick)) {
                            // value was changed
                        }

                        ImGui::SameLine();

                        static float val2 = 0;
                        if (ImGuiKnobs::Knob("Mix", &val2, -1.0f, 1.0f, 0.1f, "%.1f", ImGuiKnobVariant_Stepped)) {
                            // value was changed
                        }

                        // Double click to reset
                        if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0)) {
                            val2 = 0;
                        }

                        ImGui::SameLine();

                        static float val3 = 0;

                        // Custom colors
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(255.f, 0, 0, 0.7f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(255.f, 0, 0, 1));
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 255.f, 0, 1));
                        // Push/PopStyleColor() for each colors used (namely ImGuiCol_ButtonActive and ImGuiCol_ButtonHovered for primary and ImGuiCol_Framebg for Track)
                        if (ImGuiKnobs::Knob("Pitch", &val3, -6.0f, 6.0f, 0.1f, "%.1f", ImGuiKnobVariant_WiperOnly)) {
                            // value was changed
                        }

                        ImGui::PopStyleColor(3);


                        ImGui::SameLine();

                        // Custom min/max angle
                        static float val4 = 0;
                        if (ImGuiKnobs::Knob("Dry", &val4, -6.0f, 6.0f, 0.1f, "%.1f", ImGuiKnobVariant_Stepped, 0, 0, 10, 1.570796f, 3.141592f)) {
                            // value was changed
                        }

                        ImGui::SameLine();

                        // Int value
                        static int val5 = 1;
                        if (ImGuiKnobs::KnobInt("Wet", &val5, 1, 10, 0.1f, "%i", ImGuiKnobVariant_Stepped, 0, 0, 10)) {
                            // value was changed
                        }

                        ImGui::SameLine();

                        // Vertical drag only
                        static float val6 = 1;
                        if (ImGuiKnobs::Knob("Vertical", &val6, 0.f, 10.f, 0.1f, "%.1f", ImGuiKnobVariant_Space, 0, ImGuiKnobFlags_DragVertical)) {
                            // value was changed
                        }

                        ImGui::SameLine();

                        static float val7 = 500.0f;
                        if (ImGuiKnobs::Knob("Logarithmic", &val7, 20, 20000, 20.0f, "%.1f", ImGuiKnobVariant_WiperOnly, 0, ImGuiKnobFlags_Logarithmic | ImGuiKnobFlags_AlwaysClamp)) {
                            // value was changed
                        }

                        ImGui::End();
                    }

                    if (false) // #TODO bx de-allocation null pointer exception
                    {
                        // ImGuiIO& io = ImGui::GetIO(); // (void)io;?
                        // const ImVec2 window_size = io.DisplaySize - ImVec2(1, 1);
                        // const ImVec2 window_pos = ImVec2(1, 1);
                        // const ImVec2 node_editor_size = window_size - ImVec2(16, 16);
                        // neditor.set_size(node_editor_size);

                        ImGui::Begin("Node Editor", nullptr, 0); // ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
                        neditor.set_size(ImGui::GetContentRegionAvail());
                        neditor.draw();
                        ImGui::End();
                    }

                    if (false)
                    {
                        if (ImGui::BeginCoolBar("##CoolBarMain", ImCoolBarFlags_::ImCoolBarFlags_Horizontal, ImVec2(0.5f, 1.0f))) {
                            if (ImGui::CoolBarItem()) {
                                if (coolbar_button("A")) {

                                }
                            }
                            if (ImGui::CoolBarItem()) {
                                if (coolbar_button("B")) {

                                }
                            }
                            if (ImGui::CoolBarItem()) {
                                if (coolbar_button("C")) {

                                }
                            }
                            if (ImGui::CoolBarItem()) {
                                if (coolbar_button("D")) {

                                }
                            }
                            if (ImGui::CoolBarItem()) {
                                if (coolbar_button("E")) {

                                }
                            }
                            if (ImGui::CoolBarItem()) {
                                if (coolbar_button("F")) {

                                }
                            }
                            if (ImGui::CoolBarItem()) {
                                if (coolbar_button("G")) {

                                }
                            }
                            if (ImGui::CoolBarItem()) {
                                if (coolbar_button("H")) {

                                }
                            }
                            if (ImGui::CoolBarItem()) {
                                if (coolbar_button("I")) {

                                }
                            }
                            if (ImGui::CoolBarItem()) {
                                if (coolbar_button("J")) {

                                }
                            }
                            if (ImGui::CoolBarItem()) {
                                if (coolbar_button("K")) {

                                }
                            }
                            if (ImGui::CoolBarItem()) {
                                if (coolbar_button("L")) {

                                }
                            }
                            if (ImGui::CoolBarItem()) {
                                if (coolbar_button("M")) {

                                }
                            }
                            ImGui::EndCoolBar();
                        }
                    }

                    if (false)
                    {
                        ImGui::Begin("Progress Indicators");

                        const ImU32 col = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
                        const ImU32 bg = ImGui::GetColorU32(ImGuiCol_Button);

                        const float indicator_radius = 45.f;
                        const ImVec4 main_color = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
                        const ImVec4 backdrop_color = ImVec4(0.25f, .75f, 1.0f, 1.0f);

                        const int circle_count = 25;
                        static float speed = ImGui::GetTime() * 10.f;

                        ImGuiKnobs::Knob("IndicatorSpeedKnob", &speed, 0.f, 10.f, .05f);
                        ImGui::SameLine();
                        ImGui::LoadingIndicatorCircle("##LoadingCirclesIndicator", indicator_radius,
                            main_color, backdrop_color,
                            circle_count, speed);

                        ImGui::Spinner("##spinner", 15, 6, col);
                        ImGui::SameLine();
                        ImGui::Text("Loading %c", "|/-\\"[(int)(ImGui::GetTime() / 0.05f) & 3]);
                        ImGui::BufferingBar("##buffer_bar", 0.7f, ImVec2(400, 6), bg, col);
                        ImGui::End();
                    }

                    if (false)
                    {
                        ImGui::Begin("Sequencer");
                        const bool deleteKeyframe = ImGui::Button("Delete");
                        if (ImGui::BeginNeoSequencer("Sequencer1", &currentFrame, &startFrame, &endFrame, { 0, 0 },
                            ImGuiNeoSequencerFlags_EnableSelection |
                            ImGuiNeoSequencerFlags_Selection_EnableDragging |
                            ImGuiNeoSequencerFlags_Selection_EnableDeletion))
                        {
                            if (ImGui::BeginNeoGroup("Transform", &transformOpen))
                            {

                                if (ImGui::BeginNeoTimelineEx("Position"))
                                {
                                    for (auto&& v : keys)
                                    {
                                        ImGui::NeoKeyframe(&v);
                                        // Per keyframe code here
                                    }

                                    if (deleteKeyframe)
                                    {
                                        uint32_t count = ImGui::GetNeoKeyframeSelectionSize();

                                        ImGui::FrameIndexType* toRemove = new ImGui::FrameIndexType[count];

                                        ImGui::GetNeoKeyframeSelection(toRemove);

                                        //Delete keyframes from your structure
                                    }
                                    ImGui::EndNeoTimeLine();
                                }
                                ImGui::EndNeoGroup();
                            }

                            ImGui::EndNeoSequencer();
                        }
                        ImGui::End();
                    }

                    if (Input::IsJoystickButtonDown(eKeys::eKeys_JoystickA))
                    {
                        int bp = 0; // #TODO Refactor Input::
                    }

                    if (false)
                    {
                        // #TODO Fix m_getIcon->CreateTexture() runtime crash
                        // Review: https://github.com/dfranx/ImFileDialog#usage

                        // Simple window
                        ImGui::Begin("Control Panel");
                        if (ImGui::Button("Open file"))
                            ifd::FileDialog::Instance().Open("ShaderOpenDialog", "Open a shader", "Image file (*.png;*.jpg;*.jpeg;*.bmp;*.tga){.png,.jpg,.jpeg,.bmp,.tga},.*", true);
                        if (ImGui::Button("Open directory"))
                            ifd::FileDialog::Instance().Open("DirectoryOpenDialog", "Open a directory", "");
                        if (ImGui::Button("Save file"))
                            ifd::FileDialog::Instance().Save("ShaderSaveDialog", "Save a shader", "*.sprj {.sprj}");
                        ImGui::End();

                        // File dialogs
                        if (ifd::FileDialog::Instance().IsDone("ShaderOpenDialog")) {
                            if (ifd::FileDialog::Instance().HasResult()) {
                                const std::vector<std::filesystem::path>& res = ifd::FileDialog::Instance().GetResults();
                                for (const auto& r : res) // ShaderOpenDialog supports multiselection
                                    printf("OPEN[%s]\n", r.u8string().c_str());
                            }
                            ifd::FileDialog::Instance().Close();
                        }
                        if (ifd::FileDialog::Instance().IsDone("DirectoryOpenDialog")) {
                            if (ifd::FileDialog::Instance().HasResult()) {
                                std::string res = ifd::FileDialog::Instance().GetResult().u8string();
                                printf("DIRECTORY[%s]\n", res.c_str());
                            }
                            ifd::FileDialog::Instance().Close();
                        }
                        if (ifd::FileDialog::Instance().IsDone("ShaderSaveDialog")) {
                            if (ifd::FileDialog::Instance().HasResult()) {
                                std::string res = ifd::FileDialog::Instance().GetResult().u8string();
                                printf("SAVE[%s]\n", res.c_str());
                            }
                            ifd::FileDialog::Instance().Close();
                        }
                    }

                    if (false)
                    {
                        ImGui::Begin("Pie");
                        {
                            static const char* test_data = "Menu";
                            const char* items[] = { "Orange", "Blue", "Purple", "Gray", "Yellow", "Las Vegas" };
                            int items_count = sizeof(items) / sizeof(*items);

                            static int selected = -1;

                            ImGui::Button(selected >= 0 ? items[selected] : "Menu", ImVec2(50, 50));
                            if (ImGui::IsItemActive())          // Don't wait for button release to activate the pie menu
                                ImGui::OpenPopup("##piepopup");

                            ImVec2 pie_menu_center = ImGui::GetIO().MouseClickedPos[0];
                            int n = ImGui::PiePopupSelectMenu(pie_menu_center, "##piepopup", items, items_count, &selected);
                            if (n >= 0)
                                printf("returned %d\n", n);
                        }
                        ImGui::End();
                    }

                    if (false)
                    {
                        static bool isOn = true;
                        ImGui::ToggleButton("ToggleMe", &isOn);
                    }

                    if (false)
                    {
                        ImGui::Begin("Spinners");
                        ImSpinner::demoSpinners();
                        ImGui::End();
                    }

                    if (false)
                    {
                        // ImGui::SameLine();
                        // ImGui::PushID(18);
                        // ImZoomSlider::ImZoomSlider(0.f, 1.f, vMin, vMax, 0.01f, ImZoomSlider::ImGuiZoomSliderFlags_Vertical);
                        // ImGui::PopID();
                    }

                    if (false)
                    {
                        ImGui::Begin("Toggles");
                        imgui_toggle_example();
                        imgui_toggle_simple();
                        imgui_toggle_custom();
                        // imgui_toggle_state(const ImGuiToggleConfig & config, ImGuiToggleStateConfig & state);
                        ImGui::End();
                    }

                    if (true)
                    {
                        DrawNotifyWindow();
                    }

                    Renderer::EndImGui();

                    local_EndFrame();
                    // #TODO Review Framework::EndFrame();
				}
				else
				{
					YieldProcessor();
				}
			}

			Settings::SaveEngineSettings();

            // #TODO Save editor state for next launch

			Framework::Shutdown();
            local_Shutdown();
		}

        void DrawNotifyWindow()
        {
            ImGui::SetNextWindowPos({ 50, 50 }, ImGuiCond_Once);
            ImGui::SetNextWindowSize({ 550, 550 }, ImGuiCond_Once);
            ImGui::Begin("Hello World!", NULL, NULL);

            if (ImGui::CollapsingHeader("Examples without title", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::Button("Success"))
                {
                    ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Hello World! This is a success! %s", "We can also format here:)" });
                }

                ImGui::SameLine();
                if (ImGui::Button("Warning"))
                {
                    ImGui::InsertNotification({ ImGuiToastType_Warning, 3000, "Hello World! This is a warning!" });
                }

                ImGui::SameLine();
                if (ImGui::Button("Error"))
                {
                    ImGui::InsertNotification({ ImGuiToastType_Error, 3000, "Hello World! This is an error!" });
                }

                ImGui::SameLine();
                if (ImGui::Button("Loading"))
                {
                    ImGui::InsertNotification({ ImGuiToastType_Loading, 3000, "Texture: Dummy.txt" });
                }

                ImGui::SameLine();
                if (ImGui::Button("Info"))
                {
                    ImGui::InsertNotification({ ImGuiToastType_Info, 3000, "Hello World! This is an info!" });
                }

                ImGui::SameLine();
                if (ImGui::Button("Info long"))
                {
                    ImGui::InsertNotification({ ImGuiToastType_Info, 3000, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation" });
                }
            }

            if (ImGui::CollapsingHeader("Do it yourself", ImGuiTreeNodeFlags_DefaultOpen))
            {
                static char title[4096] = "A wonderful quote!";
                ImGui::InputTextMultiline("Title", title, sizeof(title));

                static char content[4096] = "Ours is a friendship forged once in this life, and again in the next. Goodbye, my brother. \n- Dembe Zuma";
                ImGui::InputTextMultiline("Content", content, sizeof(content));

                static int duration = 5000; // 5 seconds
                ImGui::InputInt("Duration (ms)", &duration, 100);
                if (duration < 0) duration = 0; // Shouldn't be negative

                static char* type_str[] = { "None", "Success", "Warning", "Error", "Info", "Loading"};
                static ImGuiToastType type = ImGuiToastType_Success;
                IM_ASSERT(type < ImGuiToastType_COUNT);

                if (ImGui::BeginCombo("Type", type_str[type]))
                {
                    for (auto n = 0; n < IM_ARRAYSIZE(type_str); n++)
                    {
                        const bool is_selected = (type == n);

                        if (ImGui::Selectable(type_str[n], is_selected))
                            type = n;

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                static bool enable_title = true, enable_content = true;
                ImGui::Checkbox("Enable title", &enable_title);
                ImGui::SameLine();
                ImGui::Checkbox("Enable content", &enable_content);

                if (ImGui::Button("Show"))
                {
                    ImGuiToast toast(type, duration);

                    if (enable_title)
                        toast.set_title(title);

                    if (enable_content)
                        toast.set_content(content);

                    ImGui::InsertNotification(toast);
                }
            }

            ImGui::End();

            // Render toasts on top of everything, at the end of your code!
            // You should push style vars here
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
            ImGui::RenderNotifications();
            ImGui::PopStyleVar(1); // Don't forget to Pop()
            ImGui::PopStyleColor(1);
        }

        const EditorStateFlags& GetEditorStateFlags()
        {
            return s_EditorStateFlags;
        }

        void SetEditorStateFlags(const EditorStateFlags& flags)
        {
            s_EditorStateFlags = (EditorStateFlags)(s_EditorStateFlags & flags);
        }

        void ToggleEditorStateFlags(const EditorStateFlags& flags)
        {
            s_EditorStateFlags = (EditorStateFlags)(s_EditorStateFlags ^ flags);
        }

        void RequestRestart()
        {
            s_ReloadRequested = true;
            Window::RequestClose();
        }

        bool ShowingEditorUI()
        {
            return s_ShowingEditorUI;
        }

        void NewEditorWindow(u32 enumToInt)
        {
            EditorWindowTypes editorWindowType = EditorWindowTypes::_from_index(enumToInt);

            if (editorWindowType == s_EditorWindowDockingContext.Type())
            {
                // s_EditorWindowDockingContext.ToggleHidden();
                return;
            }

            for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
            {
                if (s_FocusedWindowsStack[i]->WindowFlags() & EditorWindowFlags::Singleton &&
                    s_FocusedWindowsStack[i]->Type() == enumToInt)
                {
                    s_FocusedWindowsStack[i]->ToggleHidden();
                    // #TODO Set editor UI state to dirty
                    return;
                }
            }

            if (EditorWindow* newWindow = NewEditorWindowByType(EditorWindowsList{}, editorWindowType))
            {
                s_FocusedWindowsStack.insert(s_FocusedWindowsStack.begin(), newWindow);
                OnEditorWindowFocused(newWindow);
            }
        }

        void CloseEditorWindow(const GUID& guid)
        {
            bool closed = false;
            for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
            {
                if (guid == s_FocusedWindowsStack[i]->Guid())
                {
                    if (EditorWindowTypes::Console == (s8)s_FocusedWindowsStack[i]->Type() ||
                        EditorWindowTypes::MenuBar == (s8)s_FocusedWindowsStack[i]->Type())
                    {
                        // #TODO Handle deleting console window properly
                        // #TODO Decide what windows can be closed, or just hidden (menu bar, docking, etc)
                        return;
                    }
                    delete s_FocusedWindowsStack[i];
                    s_FocusedWindowsStack.erase(s_FocusedWindowsStack.begin() + i);
                    return;
                }
            }

            LOG_WARN("{0} Could not close editor window with GUID {1}", __FUNCTION__, guid);
        }

        const std::vector<EditorWindow*>& GetOpenWindows()
        {
            return s_FocusedWindowsStack;
        }

        void OnEditorWindowFocused(const EditorWindow* const focusedWindow)
        {
            ASSERT(focusedWindow, "Null focusedWindow pointer!");

            s16 index = -1;
            for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
            {
                if (focusedWindow == s_FocusedWindowsStack[i])
                {
                    index = i;
                    break;
                }
            }

            // Update stack
            if (0 == index)
            {
                return;
            }
            else if (index >= 0)
            {
                for (size_t i = index; i > 0; --i)
                {
                    EditorWindow* cache = s_FocusedWindowsStack[i];
                    s_FocusedWindowsStack[i] = s_FocusedWindowsStack[i - 1];
                    s_FocusedWindowsStack[i - 1] = cache;
                }
            }
        }

        void OnEntitySelected(EntityHandle& entity)
        {
            for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
            {
                s_FocusedWindowsStack[i]->OnEntitySelected(entity);
            }
        }

        void OnSceneReloaded()
        {
            for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
            {
                s_FocusedWindowsStack[i]->OnSceneReload();
            }
        }

		void local_Initialize()
		{
            Projects::Initialize();

            LoadImGuiStyleFromFile();

            const std::string windowsDataFilePath = Paths::Setting(s_EditorWindowDataFileName);
            if (!Files::Exists(windowsDataFilePath.c_str()))
            {
                Serialize::ToFile(s_FocusedWindowsStack, Paths::Setting(s_EditorWindowDataFileName).c_str());
            }
            s_FocusedWindowsStack.reserve(40);
            Serialize::FromFile(Paths::Setting(s_EditorWindowDataFileName).c_str(), s_FocusedWindowsStack);

            bool missingMenuBarWindow = true;
            for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
            {
                if (EditorWindowTypes::MenuBar == (u32)s_FocusedWindowsStack[i]->Type())
                {
                    missingMenuBarWindow = false;
                    break;
                }
            }

            if (missingMenuBarWindow)
            {
                NewEditorWindow(EditorWindowTypes::MenuBar);
            }

            // #TODO Move to Settings::Initialize()?
            const EngineSettings& engineSettings = Settings::GetEngineSettings();
            Time::SetMaximumFramerate(engineSettings.limitFramerate ? engineSettings.maxFramesPerSecond : engineSettings.defaultMaxFramesPerSecond);
		}

		void local_Shutdown()
		{
            Projects::Shutdown();
            // #TODO Save loaded data information
            // s_EditorLastOpenedData.LastUserSettingsFileName = Settings::GetUserSettings();
            Serialize::ToFile(s_FocusedWindowsStack, Paths::Setting(s_EditorWindowDataFileName).c_str()); // #TODO s_EditorWindowDataFileName everywhere

            for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
            {
                delete s_FocusedWindowsStack[i];
            }
            s_FocusedWindowsStack.clear();
		}

        void local_Update()
        {
            if (Input::FrameKeyAction(eKeys::eKeys_Escape, eKeyState::eKeyState_Press))
            {
                local_Stop();
            }

            if (!s_ShowingWindowStackPanel)
            {
                if (Input::FrameKeyAction(eKeys::eKeys_Tab, eKeyState::eKeyState_Press) &&
                    Input::IsKeyDown(eKeys::eKeys_LCTRL))
                {
                    s_ShowingWindowStackPanel = true;
                }
            }
            else if (s_ShowingWindowStackPanel = Input::IsKeyDown(eKeys::eKeys_LCTRL))
            {
                // #TODO Look into newer Tables API
                // if (ImGui::BeginTable("Window Stacks Table", 2, ImGuiTableFlags_None))
                // {
                //     ImGui::TableSetupColumn("Window Stack", ImGuiTableColumnFlags_None);
                //     ImGui::TableSetupColumn("Open Windows", ImGuiTableColumnFlags_None);
                //     ImGui::TableHeadersRow();
                //
                //     ImGui::TableSetColumnIndex(0);
                //     for (const auto& guidWindowPtrPairs : s_EditorWindows)
                //     {
                //         ImGui::TableNextRow();
                //         if (!s_WindowStackPanelLastSelected)
                //         {
                //             s_WindowStackPanelLastSelected = guidWindowPtrPairs.second;
                //         }
                //
                //         if (guidWindowPtrPairs.second->WindowFlags() ^ EditorWindowFlags::Hidden)
                //         {
                //             bool selected = false;
                //             if (ImGui::Selectable((guidWindowPtrPairs.second->Name() + "##Selectable").data(), &selected, ImGuiSelectableFlags_SelectOnNav, ImVec2(ImGui::GetContentRegionAvail().x, 25.f)))
                //             {
                //                 s_WindowStackPanelLastSelected = guidWindowPtrPairs.second;
                //             }
                //         }
                //     }
                //
                //     ImGui::TableSetColumnIndex(1);
                //     for (const auto& guidWindowPtrPairs : s_EditorWindows)
                //     {
                //         ImGui::TableNextRow();
                //         if (guidWindowPtrPairs.second->WindowFlags() ^ EditorWindowFlags::Hidden)
                //         {
                //             if (ImGui::ButtonEx((guidWindowPtrPairs.second->Name() + "##Button").data(), ImVec2(ImGui::GetContentRegionAvail().x, 25.f), ImGuiButtonFlags_NoNavFocus | ImGuiButtonFlags_NoHoveredOnFocus))
                //             {
                //                 guidWindowPtrPairs.second->Focus();
                //                 s_ShowingWindowStackPanel = false;
                //             }
                //         }
                //     }
                //
                //     ImGui::EndTable();
                // }

                const vec2f& size = Window::GetSize();
                ImGui::SetNextWindowSizeConstraints(ImVec2(0.f, 0.f), ImVec2(size.x * 0.3f, size.y * .7f));

                bool isOpen = true;
                if (!ImGui::Begin("Editor Window Stack Panel", &isOpen,
                    ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoDocking |
                    ImGuiWindowFlags_NoDecoration |
                    ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_AlwaysAutoResize |
                    (s_FocusedWindowsStack.size() > 25 ? ImGuiWindowFlags_AlwaysVerticalScrollbar : 0)
                ))
                {
                    ImGui::End();
                }

                float spacing = ImGui::GetWindowWidth() / 2 - (strlen("Windows Stacks") / 2 * ImGui::g_pixelsPerCharacter);
                ImGui::SameLine(0.f, spacing);
                ImGui::TextUnformatted("Windows Stack");

                ImGui::Separator(); // "Windows Stack"
                for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
                {
                    if (!s_WindowStackPanelLastSelected)
                    {
                        s_WindowStackPanelLastSelected = s_FocusedWindowsStack[i];
                    }

                    if (s_FocusedWindowsStack[i]->WindowFlags() ^ EditorWindowFlags::Hidden)
                    {
                        bool selected = false;

                        // if (ImGui::ButtonEx((s_FocusedWindowsStack[i]->Name() + "##Selectable").data(), ImVec2(ImGui::GetContentRegionAvail().x, 25.f), imguibutton))
                        // {
                        //     s_WindowStackPanelLastSelected = s_FocusedWindowsStack[i];
                        // }
                        if (ImGui::Selectable((s_FocusedWindowsStack[i]->Name() + "##Selectable").data(), &selected, ImGuiSelectableFlags_SelectOnNav, ImVec2(ImGui::GetContentRegionAvail().x, 25.f)))
                        {
                            s_WindowStackPanelLastSelected = s_FocusedWindowsStack[i];
                        }
                        else if (ImGui::IsItemClicked())
                        {
                            s_WindowStackPanelLastSelected = s_FocusedWindowsStack[i];
                            s_WindowStackPanelLastSelected->Focus();
                        }
                    }
                }
                ImGui::End();
            }
            else // Changed since last frame (CTRL released this frame)
            {
                if (s_WindowStackPanelLastSelected)
                {
                    s_WindowStackPanelLastSelected->Focus();
                }
            }

            if (Input::FrameKeyAction(eKeys::eKeys_R, eKeyState::eKeyState_Press) &&
                Input::IsKeyDown(eKeys::eKeys_LCTRL))
            {
                RequestRestart();
            }

            if (Input::FrameKeyAction(eKeys::eKeys_U, eKeyState::eKeyState_Press) &&
                Input::IsKeyDown(eKeys::eKeys_LCTRL))
            {
                s_ShowingEditorUI = !s_ShowingEditorUI;
            }

            constexpr size_t numberOfHotkeyedScenes = eKeys::eKeys_F12 - eKeys::eKeys_F1 + 1;
            for (size_t i = 0; i < numberOfHotkeyedScenes; i++)
            {
                if (Input::FrameKeyAction((eKeys)(eKeys::eKeys_F1 + i), eKeyState::eKeyState_Press))
                {
                    Scenes::SetCurrentScene((int)i);
                    // #NOTE Scene transition changes
                    // #TODO SetActive(true/false)
                    // Scenes::SetCurrentScene((int)i);
                    break;
                }
            }

            for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
            {
                s_FocusedWindowsStack[i]->Draw();
            }
        }

        void local_EndFrame()
        {
            if (!Window::IsMinimized())
            {
                // #TODO Replace editor draws by moving to EditorWindowSceneView class
                Debug::DrawSphere(vec3f(.0f, .0f, .0f), 0.1f);
                Debug::DrawGrid(vec3f(.0f, .0f, .0f), 50);
                // Framework::RenderView(viewIdFbo1);
            }
            Framework::EndFrame();
        }

        bool local_StillRunning()
        {
            return !Window::CloseRequested();
        }

        void local_Stop()
        {
            Window::RequestClose();
        }

        void local_FileDropCallback(const char* filePath)
        {
            const Path fileName = Files::FileName(filePath);
            const Path fileExtension = Files::FileExtension(filePath);

            if (strcmp(fileExtension.string().c_str(), ".qproj"))
            {
                std::string projectFilePath = Paths::Project(fileName.string().c_str());
                if (Files::Exists(projectFilePath.c_str()))
                {
                    Projects::LoadProject(fileName.string());
                }
            }
            else
            {
                LOG_WARN("Drag file type unsupported: {0}", fileExtension.string().c_str());
            }
        }

	}

}

// From: https://github.com/cmdwtf/imgui_toggle/blob/main/EXAMPLE.md
static void imgui_toggle_example()
{
    // use some lovely gray backgrounds for "off" toggles
    // the default would otherwise use your theme's frame background colors.
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.45f, 0.45f, 0.45f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.65f, 0.65f, 0.65f, 1.0f));

    // a toggle that will allow the user to view the demo for simple toggles or a custom toggle
    static bool show_custom_toggle = true;
    ImGui::Toggle(
        show_custom_toggle ? "Showing Custom Toggle" : "Showing Simple Toggles"
        , &show_custom_toggle);

    ImGui::Separator();

    if (show_custom_toggle)
    {
        imgui_toggle_custom();
    }
    else
    {
        imgui_toggle_simple();
    }

    // pop the color styles
    ImGui::PopStyleColor(2);
}

static void imgui_toggle_simple()
{
    static bool values[] = { true, true, true, true, true, true, true, true };
    size_t value_index = 0;

    const ImVec4 green(0.16f, 0.66f, 0.45f, 1.0f);
    const ImVec4 green_hover(0.0f, 1.0f, 0.57f, 1.0f);
    const ImVec4 salmon(1.0f, 0.43f, 0.35f, 1.0f);
    const ImVec4 green_shadow(0.0f, 1.0f, 0.0f, 0.4f);

    // a default and default animated toggle
    ImGui::Toggle("Default Toggle", &values[value_index++]);
    ImGui::Toggle("Animated Toggle", &values[value_index++], ImGuiToggleFlags_Animated);

    // this toggle draws a simple border around it's frame and knob
    ImGui::Toggle("Bordered Knob", &values[value_index++], ImGuiToggleFlags_Bordered, 1.0f);

    // this toggle draws a simple shadow around it's frame and knob
    ImGui::PushStyleColor(ImGuiCol_BorderShadow, green_shadow);
    ImGui::Toggle("Shadowed Knob", &values[value_index++], ImGuiToggleFlags_Shadowed, 1.0f);

    // this toggle draws the shadow & and the border around it's frame and knob.
    ImGui::Toggle("Bordered + Shadowed Knob", &values[value_index++], ImGuiToggleFlags_Bordered | ImGuiToggleFlags_Shadowed, 1.0f);
    ImGui::PopStyleColor(1);

    // this toggle uses stack-pushed style colors to change the way it displays
    ImGui::PushStyleColor(ImGuiCol_Button, green);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, green_hover);
    ImGui::Toggle("Green Toggle", &values[value_index++]);
    ImGui::PopStyleColor(2);

    ImGui::Toggle("Toggle with A11y Labels", &values[value_index++], ImGuiToggleFlags_A11y);

    // this toggle shows no label
    ImGui::Toggle("##Toggle With Hidden Label", &values[value_index++]);
}

static void imgui_toggle_custom()
{
    static ImGuiToggleConfig config;
    static bool toggle_custom = true;

    ImGui::NewLine();

    ImGui::Toggle("Customized Toggle", &toggle_custom, config);

    ImGui::NewLine();

    // these first settings are used no matter the toggle's state.
    ImGui::Text("Persistent Toggle Settings");

    // animation duration controls how long the toggle animates, in seconds. if set to 0, animation is disabled.
    if (ImGui::SliderFloat("Animation Duration (seconds)", &config.AnimationDuration, ImGuiToggleConstants::AnimationDurationMinimum, 2.0f))
    {
        // if the user adjusted the animation duration slider, go ahead and turn on the animation flags.
        config.Flags |= ImGuiToggleFlags_Animated;
    }

    // frame rounding sets how round the frame is when drawn, where 0 is a rectangle, and 1 is a circle.
    ImGui::SliderFloat("Frame Rounding (scale)", &config.FrameRounding, ImGuiToggleConstants::FrameRoundingMinimum, ImGuiToggleConstants::FrameRoundingMaximum);

    // knob rounding sets how round the knob is when drawn, where 0 is a rectangle, and 1 is a circle.
    ImGui::SliderFloat("Knob Rounding (scale)", &config.KnobRounding, ImGuiToggleConstants::KnobRoundingMinimum, ImGuiToggleConstants::KnobRoundingMaximum);

    // size controls the width and the height of the toggle frame
    ImGui::SliderFloat2("Size (px: w, h)", &config.Size.x, 0.0f, 200.0f, "%.0f");

    // width ratio sets how wide the toggle is with relation to the frame height. if Size is non-zero, this is unused.
    ImGui::SliderFloat("Width Ratio (scale)", &config.WidthRatio, ImGuiToggleConstants::WidthRatioMinimum, ImGuiToggleConstants::WidthRatioMaximum);

    // a11y style sets the type of additional on/off indicator drawing
    if (ImGui::Combo("A11y Style", &config.A11yStyle,
        "Label\0"
        "Glyph\0"
        "Dot\0"
        "\0"))
    {
        // if the user adjusted the a11y style combo, go ahead and turn on the a11y flag.
        config.Flags |= ImGuiToggleFlags_A11y;
    }

    // some tabs to adjust the "state" settings of the toggle (configuration dependent on if the toggle is on or off.)
    if (ImGui::BeginTabBar("State"))
    {
        if (ImGui::BeginTabItem("\"Off State\" Settings"))
        {
            imgui_toggle_state(config, config.Off);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("\"On State\"Settings"))
        {
            imgui_toggle_state(config, config.On);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::Separator();

    // flags for various toggle features
    ImGui::Text("Flags");
    ImGui::Columns(2);
    ImGui::Text("Meta Flags");
    ImGui::NextColumn();
    ImGui::Text("Individual Flags");
    ImGui::Separator();
    ImGui::NextColumn();

    // should the toggle have borders (sets all border flags)
    ImGui::CheckboxFlags("Bordered", &config.Flags, ImGuiToggleFlags_Bordered);

    // should the toggle have shadows (sets all shadow flags)
    ImGui::CheckboxFlags("Shadowed", &config.Flags, ImGuiToggleFlags_Shadowed);

    ImGui::NextColumn();

    // should the toggle animate
    ImGui::CheckboxFlags("Animated", &config.Flags, ImGuiToggleFlags_Animated);

    // should the toggle have a bordered frame
    ImGui::CheckboxFlags("BorderedFrame", &config.Flags, ImGuiToggleFlags_BorderedFrame);

    // should the toggle have a bordered knob
    ImGui::CheckboxFlags("BorderedKnob", &config.Flags, ImGuiToggleFlags_BorderedKnob);

    // should the toggle have a shadowed frame
    ImGui::CheckboxFlags("ShadowedFrame", &config.Flags, ImGuiToggleFlags_ShadowedFrame);

    // should the toggle have a shadowed knob
    ImGui::CheckboxFlags("ShadowedKnob", &config.Flags, ImGuiToggleFlags_ShadowedKnob);

    // should the toggle draw a11y glyphs
    ImGui::CheckboxFlags("A11y", &config.Flags, ImGuiToggleFlags_A11y);
    ImGui::Columns();

    ImGui::Separator();

    // what follows are some configuration presets. check the source of those functions to see how they work.
    ImGui::Text("Configuration Style Presets");

    if (ImGui::Button("Reset to Default"))
    {
        config = ImGuiTogglePresets::DefaultStyle();
    }
    ImGui::SameLine();

    if (ImGui::Button("Rectangle"))
    {
        config = ImGuiTogglePresets::RectangleStyle();
    }
    ImGui::SameLine();

    if (ImGui::Button("Glowing"))
    {
        config = ImGuiTogglePresets::GlowingStyle();
    }
    ImGui::SameLine();

    if (ImGui::Button("iOS"))
    {
        config = ImGuiTogglePresets::iOSStyle();
    }
    ImGui::SameLine();

    if (ImGui::Button("Material"))
    {
        config = ImGuiTogglePresets::MaterialStyle();
    }
    ImGui::SameLine();

    if (ImGui::Button("Minecraft"))
    {
        config = ImGuiTogglePresets::MinecraftStyle();
    }
}

static void imgui_toggle_state(const ImGuiToggleConfig& config, ImGuiToggleStateConfig& state)
{
    // some values to use for slider limits
    const float border_thickness_max_pixels = 50.0f;
    const float max_height = config.Size.y > 0 ? config.Size.y : ImGui::GetFrameHeight();
    const float half_max_height = max_height * 0.5f;

    // knob offset controls how far into or out of the frame the knob should draw.
    ImGui::SliderFloat2("Knob Offset (px: x, y)", &state.KnobOffset.x, -half_max_height, half_max_height);

    // knob inset controls how many pixels the knob is set into the frame. negative values will cause it to grow outside the frame.
    // for circular knobs, we will just use a single value, while for we will use top/left/bottom/right offsets.
    const bool is_rounded = config.KnobRounding >= 1.0f;
    if (is_rounded)
    {
        float inset_average = state.KnobInset.GetAverage();
        ImGui::SliderFloat("Knob Inset (px)", &inset_average, -half_max_height, half_max_height);
        state.KnobInset = inset_average;
    }
    else
    {
        ImGui::SliderFloat4("Knob Inset (px: t, l, b, r)", state.KnobInset.Offsets, -half_max_height, half_max_height);
    }

    // how thick should the frame border be (if enabled)
    ImGui::SliderFloat("Frame Border Thickness (px)", &state.FrameBorderThickness, 0.0f, border_thickness_max_pixels);

    // how thick should the knob border be (if enabled)
    ImGui::SliderFloat("Knob Border Thickness (px)", &state.KnobBorderThickness, 0.0f, border_thickness_max_pixels);
}

void main(unsigned int numberOfArguments, char** commandLineArguments)
{
    QwerkE::Editor::Run(numberOfArguments, commandLineArguments);
}
