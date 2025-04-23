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
#include "Libraries/imgui-node-editor/imgui_node_editor.h"
#include "Libraries/imgui-notify/imgui_notify.h"
#include "Libraries/im-neo-sequencer/imgui_neo_sequencer.h"
#include "Libraries/ImGuizmo/ImGuizmo.h"
#include "Libraries/ImNodeFlow/ImNodeFlow.h"
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

// From: https://gitlab.com/GroGy/im-neo-sequencer
int32_t currentFrame = 0; // Sequencer
int32_t startFrame = -10;
int32_t endFrame = 64;
bool transformOpen = false;
std::vector<ImGui::FrameIndexType> keys = { 0, 10, 24 };
bool doDelete = false;

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
                        ImGui::Begin("Progress Indicators");

                        const ImU32 col = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
                        const ImU32 bg = ImGui::GetColorU32(ImGuiCol_Button);

                        const float indicator_radius = 45.f;
                        const ImVec4 main_color = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
                        const ImVec4 backdrop_color = ImVec4(0.25f, .75f, 1.0f, 1.0f);

                        const int circle_count = 25;
                        static float speed = ImGui::GetTime() * 10.f;

                        ImGui::Knob("IndicatorSpeedKnob", &speed, 0.f, 10.f, .05f);
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
                        // ImGui::SameLine();
                        // ImGui::PushID(18);
                        // ImZoomSlider::ImZoomSlider(0.f, 1.f, vMin, vMax, 0.01f, ImZoomSlider::ImGuiZoomSliderFlags_Vertical);
                        // ImGui::PopID();
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
            Debug::DrawCube({}, 1.f, false, Debug::g_Purple);

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
                const vec2f& size = Window::GetSize();
                ImGui::SetNextWindowSizeConstraints(ImVec2(0.f, 0.f), ImVec2(size.x * 0.3f, size.y * .7f));

                bool isOpen = true;
                if (ImGui::Begin("Editor Window Stack Panel", &isOpen,
                    ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoDocking |
                    ImGuiWindowFlags_NoDecoration |
                    ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_AlwaysAutoResize |
                    (s_FocusedWindowsStack.size() > 25 ? ImGuiWindowFlags_AlwaysVerticalScrollbar : 0)
                ))
                {
                    ImGui::TextUnformatted("   Windows Stack   "); // #NOTE Sets the window width

                    ImGui::Separator(); // "Windows Stack"
                    for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
                    {
                        if (!s_WindowStackPanelLastSelected)
                        {
                            s_WindowStackPanelLastSelected = s_FocusedWindowsStack[i];
                        }

                        if (s_FocusedWindowsStack[i]->WindowFlags() ^ EditorWindowFlags::Hidden)
                        {
                            // u32 selectedIndex = 1; // #TODO Select 2nd element on open
                            static bool selected = false; // #TODO Since making static, assert has not been triggered, but focusing every 2nd item
                            // causes every selectable item to be focused

                            // #TODO Fix imgui.cpp(4061) assert where: IM_ASSERT(false || ...177 == ...628 || ...177 == 0 || ...177 == ...628);
                            // Maybe related to selected being non-static or unused in some way.
                            constexpr float itemHeight = 25.f;
                            if (ImGui::Selectable((s_FocusedWindowsStack[i]->Name() + "##Selectable").data(), &selected, ImGuiSelectableFlags_SelectOnNav, ImVec2(ImGui::GetContentRegionAvail().x, itemHeight)))
                            {
                                s_WindowStackPanelLastSelected = s_FocusedWindowsStack[i];
                                // selectedIndex = i;
                            }
                            else if (ImGui::IsItemClicked())
                            {
                                s_WindowStackPanelLastSelected = s_FocusedWindowsStack[i];
                                s_WindowStackPanelLastSelected->Focus();
                            }
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

void main(unsigned int numberOfArguments, char** commandLineArguments)
{
    QwerkE::Editor::Run(numberOfArguments, commandLineArguments);
}
