#include "QE_Editor.h"

#include <map>      // For std::map<std::string, const char*> pairs; and EditorWindows collection
#include <string>   // For std::map<std::string, const char*> pairs;
#include <typeinfo> // For typeid()
#include <vector>   // For s_EditorWindowsQueuedForDelete

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
#include "Libraries/imgui_toggle/imgui_toggle.h"
#include "Libraries/imgui_toggle/imgui_toggle_presets.h"
#include "Libraries/imgui-console/imgui_console.h"
#include "Libraries/imgui-node-editor/imgui_node_editor.h"
#include "Libraries/imgui-notify/imgui_notify.h"
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

#ifdef _QDEARIMGUI
// ImGui Console
csys::ItemLog& operator<<(csys::ItemLog& log, ImVec4& vec)
{
    log << "ImVec4: [" << vec.x << ", "
        << vec.y << ", "
        << vec.z << ", "
        << vec.w << "]";
    return log;
}

static void imvec4_setter(ImVec4& my_type, std::vector<int> vec)
{
    if (vec.size() < 4) return;

    my_type.x = vec[0] / 255.f;
    my_type.y = vec[1] / 255.f;
    my_type.z = vec[2] / 255.f;
    my_type.w = vec[3] / 255.f;
}
//
#endif

static void imgui_toggle_example();
static void imgui_toggle_simple();
static void imgui_toggle_custom();
static void imgui_toggle_state(const ImGuiToggleConfig& config, ImGuiToggleStateConfig& state);

namespace QwerkE {

    void LoadImGuiStyleFromFile() // #TODO Move somewhere else
    {
        ImGuiStyle& style = ImGui::GetStyle();
        Serialize::FromFile(Paths::Style(Settings::GetStyleFileName()).c_str(), style, true);
    }

	namespace Editor {

        static bool s_ShowingEditorUI = true;

        static EditorWindowDockingContext s_EditorWindowDockingContext(GUID::Invalid); // #NOTE Draw order dependency with other EditorWindows

        static std::unordered_map<GUID, EditorWindow*> s_EditorWindows;
        static std::vector<EditorWindow*> s_EditorWindowsQueuedForDelete;

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

        // #TODO Change to main(numberOfArguments, commandLineArguments) entry point function
		void Run(unsigned int numberOfArguments, char** commandLineArguments)
		{
            Log::Console("-- Qwerk Editor %s --\n", std::to_string(QWERKE_VERSION).c_str());

            Framework::SetCommandLineArgs(numberOfArguments, commandLineArguments); // #TODO Improve name

            Settings::LoadEngineSettings("Editor.qsetting");
            const EngineSettings& engineSettings = Settings::GetEngineSettings();
			Framework::Initialize(engineSettings.windowWidthPixels, engineSettings.windowHeightPixels);

            local_Initialize();

#ifdef _QDEARIMGUI
            ///////////////////////////////////////////////////////////////////////////
            // IMGUI CONSOLE //////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////////////

            // Our state
            ImVec4 clear_color = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);

            // Create ImGui Console
            ImGuiConsole console;

            // Register variables
            console.System().RegisterVariable("cat", clear_color, imvec4_setter);

            console.System().RegisterVariable("background_colox", clear_color, imvec4_setter);
            console.System().RegisterVariable("back", clear_color, imvec4_setter);
            console.System().RegisterVariable("back1234", clear_color, imvec4_setter);

            console.System().RegisterVariable("background_color", clear_color, imvec4_setter);
            console.System().RegisterVariable("background_color1", clear_color, imvec4_setter);
            console.System().RegisterVariable("background_color2", clear_color, imvec4_setter);
            console.System().RegisterVariable("background_color3", clear_color, imvec4_setter);
            console.System().RegisterVariable("background_color4", clear_color, imvec4_setter);
            console.System().RegisterVariable("background_color5", clear_color, imvec4_setter);

            // Register scripts
            const std::string path = "B:/QwerkE/Source/Libraries/imgui-console/console.script";
            console.System().RegisterScript("test_script", path);

            // Register custom commands
            console.System().RegisterCommand("random_background_color", "Assigns a random color to the background application",
                [&clear_color]()
                {
                    clear_color.x = (rand() % 256) / 256.f;
                    clear_color.y = (rand() % 256) / 256.f;
                    clear_color.z = (rand() % 256) / 256.f;
                    clear_color.w = (rand() % 256) / 256.f;
                });
            console.System().RegisterCommand("reset_background_color", "Reset background color to its original value",
                [&clear_color, val = clear_color]()
                {
                    clear_color = val;
                });

            // Log example information:
            console.System().Log(csys::ItemType::INFO) << "Welcome to imgui-console!" << csys::endl;

            ///////////////////////////////////////////////////////////////////////////
#endif

            Time::WriteAppStartTime();

			while (local_StillRunning())
			{
				if (Time::ShouldProcessNextFrame())
				{
                    Time::StartFrame();

					Framework::StartFrame();

                    Renderer::StartImGui();

#ifdef _QDEARIMGUI
                    // if (ImGui::Button("COMMAND"))
                    // {
                    //     console.System().Log(csys::ItemType::COMMAND) << "COMMAND";
                    // }
                    // if (ImGui::Button("LOG"))
                    // {
                    //     console.System().Log(csys::ItemType::LOG) << "LOG";
                    // }
                    // if (ImGui::Button("WARNING"))
                    // {
                    //     console.System().Log(csys::ItemType::WARNING) << "WARNING";
                    // }
                    // if (ImGui::Button("SEVERE"))
                    // {
                    //     console.System().Log(csys::ItemType::SEVERE) << "SEVERE";
                    // }
                    // if (ImGui::Button("INFO"))
                    // {
                    //     console.System().Log(csys::ItemType::LOG) << "INFO";
                    // }
                    // if (ImGui::Button("NONE"))
                    // {
                    //     console.System().Log(csys::ItemType::LOG) << "NONE";
                    // }

                    // ImGui Console
                    // console.Draw();
#endif

                    if (s_ShowingEditorUI)
                    {
                        s_EditorWindowDockingContext.Draw();
                    }

                    local_Update();

					Framework::Update((float)Time::PreviousFrameDuration());

                    static bool isOn = true;
                    ImGui::ToggleButton("ToggleMe", &isOn);

                    ImGui::Begin("Spinners");
                    ImSpinner::demoSpinners();
                    ImGui::End();

                    ImGui::Begin("Toggles");
                    imgui_toggle_example();
                    imgui_toggle_simple();
                    imgui_toggle_custom();
                    // imgui_toggle_state(const ImGuiToggleConfig & config, ImGuiToggleStateConfig & state);
                    ImGui::End();

                    DrawNotifyWindow();

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

                static char* type_str[] = { "None", "Success", "Warning", "Error", "Info" };
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

        bool ShowingEditorUI()
        {
            return s_ShowingEditorUI;
        }

        void OpenEditorWindow(u32 enumToInt)
        {
            EditorWindowTypes editorWindowType = EditorWindowTypes::_from_index(enumToInt);

            if (editorWindowType == s_EditorWindowDockingContext.Type())
            {
                // s_EditorWindowDockingContext.ToggleHidden();
                return;
            }

            for (auto& pair : s_EditorWindows)
            {
                if (pair.second->WindowFlags() & EditorWindowFlags::Singleton &&
                    pair.second->Type() == enumToInt)
                {
                    pair.second->ToggleHidden();
                    // #TODO Set editor UI state to dirty
                    return;
                }
            }

            if (EditorWindow* newWindow = NewEditorWindowByType(EditorWindowsList{}, editorWindowType))
            {
                s_EditorWindows[newWindow->Guid()] = newWindow;
            }
        }

        void CloseEditorWindow(const GUID& guid)
        {
            if (s_EditorWindows.find(guid) != s_EditorWindows.end())
            {
                s_EditorWindowsQueuedForDelete.push_back(s_EditorWindows[guid]);
            }
            else
            {
                LOG_WARN("{0} Could not close editor window with GUID {1}", __FUNCTION__, guid);
            }
        }

        void OnEntitySelected(EntityHandle& entity)
        {
            for (auto& it : s_EditorWindows)
            {
                it.second->OnEntitySelected(entity);
            }
        }

        void OnSceneReloaded()
        {
            for (auto& it : s_EditorWindows)
            {
                it.second->OnSceneReload();
            }
        }

		void local_Initialize()
		{
            Projects::Initialize();

            LoadImGuiStyleFromFile();

            const std::string windowsDataFilePath = Paths::Setting(s_EditorWindowDataFileName);
            if (!Files::Exists(windowsDataFilePath.c_str()))
            {
                Serialize::ToFile(s_EditorWindows, windowsDataFilePath.c_str());
            }
            Serialize::FromFile(windowsDataFilePath.c_str(), s_EditorWindows);

            bool missingMenuBarWindow = true;
            for (auto& pair : s_EditorWindows)
            {
                if (EditorWindowTypes::MenuBar == (u32)pair.second->Type())
                {
                    missingMenuBarWindow = false;
                }
            }

            if (missingMenuBarWindow)
            {
                OpenEditorWindow(EditorWindowTypes::MenuBar);
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
            Serialize::ToFile(s_EditorWindows, Paths::Setting(s_EditorWindowDataFileName).c_str());

            auto it = s_EditorWindows.begin();
            while (it != s_EditorWindows.end())
            {
                it = s_EditorWindows.erase(it);
            }
            s_EditorWindows.clear();
		}

        void local_Update()
        {
            if (Input::FrameKeyAction(eKeys::eKeys_Escape, eKeyState::eKeyState_Press))
            {
                local_Stop();
            }

            if (Input::FrameKeyAction(eKeys::eKeys_U, eKeyState::eKeyState_Press) &&
                (Input::IsKeyDown(eKeys::eKeys_LCTRL) || Input::IsKeyDown(eKeys::eKeys_RCTRL)))
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

            auto it = s_EditorWindows.begin();
            while (it != s_EditorWindows.end())
            {
                it->second->Draw();
                ++it;
            }

            for (size_t i = 0; i < s_EditorWindowsQueuedForDelete.size(); i++)
            {
                s_EditorWindows.erase(s_EditorWindowsQueuedForDelete[i]->Guid());
            }
            s_EditorWindowsQueuedForDelete.clear();
        }

        void local_EndFrame()
        {
            if (!Window::IsMinimized())
            {
                {   // Debug drawer calls
                    constexpr bgfx::ViewId viewIdFbo1 = 2; // #TODO Fix hard coded value
                    bgfx::setState(BGFX_STATE_DEFAULT);
                    DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer(); // #TESTING
                    debugDrawer.begin(viewIdFbo1, true);

                    constexpr bx::Vec3 normal = { .0f,  1.f, .0f };
                    constexpr bx::Vec3 pos = { .0f, .0f, .0f };

                    debugDrawer.drawSphere(0.f, 0.f, 0.f, 0.1f, Axis::X);
                    // debugDrawer.drawOrb(0.f, 0.f, 0.f, 3.f, Axis::X);

                    bx::Plane plane(bx::InitNone);
                    bx::calcPlane(plane, normal, pos);

                    debugDrawer.drawGrid(Axis::Y, pos, 50, 1.0f);

                    debugDrawer.end();
                }

                // #TODO Replace draws by moving to EditorWindowSceneView class
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
