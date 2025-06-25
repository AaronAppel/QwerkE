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

#include "QC_Guid.h"
#include "QC_ProgramArgs.h"
#include "QC_System.h"
#include "QC_Time.h"

#include "QF_Assets.h"
#include "QF_Directory.h"
#include "QF_Files.h"
#include "QF_Framework.h"

#include "QF_Input.h"
#include "QF_InputMapping.h"
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

#include "QE_EditorHotKeys.h"
#include "QE_EditorWindowHelpers.h"
#include "QE_Projects.h"

namespace QwerkE {

    void LoadImGuiStyleFromFile() // #TODO Move somewhere else
    {
        // #TODO Check for user specified style, and create one from the default if one does not exist
        ImGuiStyle& style = ImGui::GetStyle();
        Serialize::FromFile(Paths::Style(Settings::GetStyleFileName()).c_str(), style, true);
    }

	namespace Editor {

        // QE_EditorHotkeys.cpp
        extern std::unordered_map<EditorHotKeys, Input::InputMapping> s_EditorHotkeys;
        extern void PollHotkeys();

        static bool s_ShowingEditorUI = true;

        // #TODO Think of creating a WindowManager, or EditorWindows:: class to manage all window access, update, create/destroy,
        // and it can manage the stack of recently selected windows.
        static bool s_ShowingWindowStackPanel = false; // #TODO Move logic to its own window or editor file
        static EditorWindow* s_WindowStackPanelLastSelected = nullptr;

        // #TODO Editor overlays(notifications), pop ups(cycle windows, load/save project), prompts(save unsaved change before quitting/closing scene/window)
        // #TODO Replace with ring buffer or better container to avoid "bubbling"/swapping all elements
        static std::vector<EditorWindow*> s_FocusedWindowsStack = std::vector<EditorWindow*>();
        static std::vector<std::pair<bool, EditorWindow*>> s_DelayedWindowsToAddOrDelete = std::vector<std::pair<bool, EditorWindow*>>();

        static EditorWindowDockingContext s_EditorWindowDockingContext(GUID::Invalid); // #NOTE Draw order dependency with other EditorWindows

        constexpr char* s_EditorWindowDataFileName = "EditorWindowData.qdata";

        static EditorStateFlags s_EditorStateFlags = EditorStateFlags::EditorStateFlagsNone;

        static const EditorWindowEntityInspector* s_LastFocusedInspector = nullptr;
        static const EditorWindowSceneView* s_LastFocusedSceneView = nullptr;

        void local_Initialize();
        void local_Shutdown();
        void local_Update();
        void local_EndFrame();

        void local_FileDropCallback(const char* filePath);

        u32 s_FramesCompleted = 0;

        bool s_ReloadRequested = false; // #TODO bgfx destructor asserts when re-initializing framework

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

            Settings::LoadEngineSettings("Editor.qsetting"); // #TODO How to load? From where and what if a default file doesn't exist?
            const EngineSettings& engineSettings = Settings::GetEngineSettings();
			Framework::Initialize(engineSettings.windowWidthPixels, engineSettings.windowHeightPixels);

            local_Initialize();

            Settings::LoadUserSettings("Aaron.qpref"); // #TODO Testing user settings. Move somewhere better
            const UserSettings& userSettings = Settings::GetUserSettings();
            const std::vector<Scene*>& scenes = Scenes::LookAtScenes();
            for (size_t i = 0; i < scenes.size(); i++)
            {
                scenes[i]->SetIsPaused(!userSettings.startInPlayMode);
            }

            Time::WriteAppStartTime();

			while (!Window::CloseRequested())
			{
				if (Time::ShouldProcessNextFrame())
				{
                    Time::StartFrame();

					Framework::StartFrame();

                    Renderer::StartImGui(); // #NOTE Can move above Framework::StartFrame(), but might be a sign of poor imgui use (calling while not inside a begin/end block/scope)

                    local_Update();

					Framework::Update((float)Time::PreviousFrameDuration());

                    Renderer::EndImGui();

                    if (!s_DelayedWindowsToAddOrDelete.empty())
                    {
                        for (size_t i = 0; i < s_DelayedWindowsToAddOrDelete.size(); i++)
                        {
                            std::pair<bool, EditorWindow*> pair = s_DelayedWindowsToAddOrDelete[i];
                            if (pair.first) // Add
                            {
                                s_FocusedWindowsStack.insert(s_FocusedWindowsStack.begin(), pair.second); // #TODO Review windows stack addition
                            }
                            else            // Delete
                            {
                                u16 index = U16_MAX;
                                for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
                                {
                                    if (pair.second == s_FocusedWindowsStack[i])
                                    {
                                        index = i;
                                        break;
                                    }
                                }

                                if (index <= s_FocusedWindowsStack.size())
                                {
                                    delete pair.second;
                                    s_FocusedWindowsStack.erase(s_FocusedWindowsStack.begin() + index);
                                }
                            }
                        }
                    }
                    s_DelayedWindowsToAddOrDelete.clear();

                    if (!s_ShowingEditorUI)
                    {
                        Scenes::DrawCurrentScene(4); // #TODO Review hard coded viewId
                    }

                    local_EndFrame();
                    // #TODO Review Framework::EndFrame();
				}
				else
				{
                    // #TODO Review sleep function
                    // From: https://github.com/ocornut/imgui/blob/docking/backends/imgui_impl_glfw.cpp#L1006
#ifdef _QWINDOWS
                    System::YieldTime();
#else
                    usleep(milliseconds * 1000);
#endif
				}
			}

			Settings::SaveEngineSettings();

            // #TODO Save editor state for next launch

			Framework::Shutdown();
            local_Shutdown();
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

        u32 CurrentFrame()
        {
            return s_FramesCompleted;
        }

        void NewEditorWindow(u32 a_EditorWindowTypeInt)
        {
            EditorWindowTypes editorWindowType = EditorWindowTypes::_from_index(a_EditorWindowTypeInt);

            if (editorWindowType == s_EditorWindowDockingContext.Type())
            {
                // s_EditorWindowDockingContext.ToggleHidden();
                return;
            }

            for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
            {
                if (s_FocusedWindowsStack[i]->WindowFlags() & EditorWindowFlags::Singleton &&
                    s_FocusedWindowsStack[i]->Type() == a_EditorWindowTypeInt)
                {
                    s_FocusedWindowsStack[i]->ToggleHidden();
                    // #TODO Set editor UI state to dirty
                    return;
                }
            }

            if (EditorWindow* newWindow = NewEditorWindowByType(EditorWindowsList{}, editorWindowType))
            {
                // #TODO Consider reversing direction and storing recently interacted windows at the back of the vector.
                // This would avoid swapping the entire list for new windows (just push instead) and deleted windows probably had focus anyways (just pop the top).
                s_DelayedWindowsToAddOrDelete.push_back({true, newWindow });
                OnEditorWindowFocused(newWindow);
            }
        }

        void OpenEditorWindowPrompt(EditorWindowPromptTypes a_PromptType)
        {
            // #TODO Close prompt type method as well.
            // #TODO This function is pretty much the same as above. The other could return a reference to the found window for this function to operate on
            // #TODO Consider storing a reference to singletons like prompt window, as done with docking context

            for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
            {
                if (s_FocusedWindowsStack[i]->WindowFlags() & EditorWindowFlags::Singleton &&
                    (u32)s_FocusedWindowsStack[i]->Type() == EditorWindowTypes::Prompt)
                {
                    EditorWindowPrompt* promptWindow = static_cast<EditorWindowPrompt*>(s_FocusedWindowsStack[i]);
                    if (promptWindow)
                    {
                        if (promptWindow->IsHidden())
                        {
                            promptWindow->ToggleHidden();
                        }
                        else if (promptWindow->GetPromptType() == a_PromptType)
                        {
                            promptWindow->ToggleHidden();
                            return; // #NOTE Currently hiding if already open with same prompt type
                        }

                        promptWindow->SetPromptType(a_PromptType);
                    }
                    return;
                }
            }

            if (EditorWindow* newWindow = NewEditorWindowByType(EditorWindowsList{}, EditorWindowTypes::Prompt))
            {
                EditorWindowPrompt* promptWindow = static_cast<EditorWindowPrompt*>(newWindow);
                promptWindow->SetPromptType(a_PromptType);
                if (promptWindow->IsHidden())
                {
                    promptWindow->ToggleHidden();
                }

                // #TODO Consider reversing direction and storing recently interacted windows at the back of the vector.
                // This would avoid swapping the entire list for new windows (just push instead) and deleted windows probably had focus anyways (just pop the top).
                s_DelayedWindowsToAddOrDelete.push_back({ true, newWindow });
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
                    if (EditorWindowTypes::Console == (u32)s_FocusedWindowsStack[i]->Type() ||
                        EditorWindowTypes::MenuBar == (u32)s_FocusedWindowsStack[i]->Type())
                    {
                        // #TODO Handle deleting console window properly
                        // #TODO Decide what windows can be closed, or just hidden (menu bar, docking, etc)
                        return;
                    }
                    s_DelayedWindowsToAddOrDelete.push_back({ false, s_FocusedWindowsStack[i] });
                    return;
                }
            }

            LOG_WARN("{0} Could not close editor window with GUID {1}", __FUNCTION__, guid);
        }

        const std::vector<EditorWindow*>& GetOpenWindows()
        {
            return s_FocusedWindowsStack;
        }

        const EditorWindow* s_LastFocusedWindow; // #TODO Move somewhere better
        void OnEditorWindowFocused(const EditorWindow* const a_FocusedWindow)
        {
            // #TODO Consider moving to a static function in EditorWindow.h
            // ASSERT(focusedWindow, "Null focusedWindow pointer!");

            for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
            {
                s_FocusedWindowsStack[i]->OnEditorWindowFocused(a_FocusedWindow);
            }

            if (a_FocusedWindow)
            {
                if (a_FocusedWindow->Type() == (u32)EditorWindowTypes::EntityInspector)
                {
                    s_LastFocusedInspector = static_cast<const EditorWindowEntityInspector*>(a_FocusedWindow);
                }
                else if (a_FocusedWindow->Type() == (u32)EditorWindowTypes::SceneView)
                {
                    s_LastFocusedSceneView = static_cast<const EditorWindowSceneView*>(a_FocusedWindow);
                }
            }

            s_LastFocusedWindow = a_FocusedWindow;
            if (nullptr == s_LastFocusedWindow) // #TODO Handle nullptr if last focused window was closed
            {
                return; // #TODO Go through stack and remove closed window?
            }

            s16 index = -1;
            for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
            {
                if (a_FocusedWindow == s_FocusedWindowsStack[i])
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

        const EditorWindowEntityInspector* LastFocusedInspectorWindow()
        {
            return s_LastFocusedInspector;
        }

        const EditorWindowSceneView* LastFocusedSceneViewWindow()
        {
            return s_LastFocusedSceneView;
        }

        const EditorWindow* GetLastFocusedWindow()
        {
            return s_LastFocusedWindow;
        }

        void OnEntitySelected(EntityHandle& entity)
        {
            for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
            {
                s_FocusedWindowsStack[i]->OnEntitySelected(entity);
            }
        }

        void OnSceneReloaded(const Scene* a_ReloadedScene)
        {
            for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
            {
                s_FocusedWindowsStack[i]->OnSceneReload(a_ReloadedScene);
            }
        }

        void TestOnKey(QKey a_Key, QKeyState a_State)
        {
            LOG_INFO("OnKey(): {0}, {1}", a_Key, a_State);
            return;

            if (QKey::e_1 == a_Key)
            {
                LOG_INFO("OnKey(): {0}, {1}", a_Key, a_State);
                Input::OnKeyStop(TestOnKey);
            }
        }

        void TestOnMouse(QKey a_Key, QKeyState a_State, float a_ScrollDelta, const vec2f& a_MousePosition)
        {
            if (QwerkE::e_MouseMove != a_Key)
            {
                LOG_INFO("TestOnMouse(): {0}, {1}, {2}, {3}, {4}", a_Key, a_State, a_ScrollDelta, a_MousePosition.x, a_MousePosition.y);
                return;
            }

            switch (a_Key)
            {
            case QwerkE::e_MouseLeft:
                LOG_INFO("TestOnMouse(): {0}, {1}, {2}, {3}, {4}", a_Key, a_State, a_ScrollDelta, a_MousePosition.x, a_MousePosition.y);
                // Input::OnMouseStop(TestOnMouse);
                break;
            case QwerkE::e_MouseRight:
                break;
            case QwerkE::e_MouseMiddle:
                break;
            case QwerkE::e_MouseButton4:
                break;
            case QwerkE::e_MouseButton5:
                break;
            case QwerkE::e_MouseButton6:
                break;
            case QwerkE::e_MouseButton7:
                break;
            case QwerkE::e_MouseButton8:
                break;
            case QwerkE::e_ScrollUp:
                LOG_INFO("TestOnMouse(): {0}, {1}, {2}, {3}, {4}", a_Key, a_State, a_ScrollDelta, a_MousePosition.x, a_MousePosition.y);
                break;
            case QwerkE::e_ScrollDown:
                LOG_INFO("TestOnMouse(): {0}, {1}, {2}, {3}, {4}", a_Key, a_State, a_ScrollDelta, a_MousePosition.x, a_MousePosition.y);
                break;
            case QwerkE::e_MouseMove:
                // LOG_INFO("TestOnMouse(): {0}, {1}, {2}, {3}, {4}", a_Key, a_State, a_ScrollDelta, a_MousePosition.x, a_MousePosition.y);
                break;
            }
        }

        void TestOnGamepad(QGamepad a_Input, QKeyState a_State, const vec2f& a_Axis12, const vec2f& a_Axis34, const vec2f& a_Axis56, QGamepad a_GamepadId)
        {
            switch (a_Input)
            {
            case QwerkE::e_GamepadA:
                LOG_INFO("OnGamepad(): {0}, {1}", a_Input, a_State, a_Axis12.x, a_Axis12.y, a_Axis34.x, a_Axis34.y, a_Axis56.x, a_Axis56.y, a_GamepadId);
                Input::OnGamepadStop(TestOnGamepad);
                break;
            case QwerkE::e_GamepadAxis01:
                LOG_INFO("TestOnGamepad(): {0}, {1}", a_Input, a_State, a_Axis12.x, a_Axis12.y, a_Axis34.x, a_Axis34.y, a_Axis56.x, a_Axis56.y, a_GamepadId);
                break;
            }
        }

        void ResolveHotkeys()
        {
            // Global application commands
            if (s_EditorHotkeys[e_CloseApplication].IsActive())
            {
                Window::RequestClose();
            }

            if (s_EditorHotkeys[e_RestartApplication].IsActive())
            {
                RequestRestart();
            }

            if (s_EditorHotkeys[e_HideEditorUi].IsActive())
            {
                s_ShowingEditorUI = !s_ShowingEditorUI;
            }

            // Editor Windows // #TODO Should windows hold their own hotkey logic? Singletons can still run while hidden
            // #TODO Stack frame window should be its own EditorWindow* class to better handle s_ShowingEditorUI and other logic
            if (s_ShowingEditorUI) // ImGui logic (must check if drawing or imgui calls can undock windows)
            {
                if (bool stackPanelEnabled = false) // #TODO Stack panel broke after connecting ImGui input, and InputMapping/s_EditorHotkeys update
                {
                    if (!s_ShowingWindowStackPanel)
                    {
                        if (s_EditorHotkeys[e_WindowStackPanelOpen].IsActive())
                        {
                            s_ShowingWindowStackPanel = true;
                        }
                    }
                    // #TODO Change to use new input mapping s_EditorHotkeys
                    else if (s_ShowingWindowStackPanel = Input::KeyDown(QKey::e_CtrlAny)) // #NOTE Assignment intentional
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
                                        // #TODO s_WindowStackPanelLastSelected->Highlight();
                                        // NavUpdateWindowingHighlightWindow()
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
                }
            }

            // Scenes
            constexpr size_t numberOfHotkeyedScenes = QKey::e_F12 - QKey::e_F1 + 1;
            for (u8 i = 0; i < numberOfHotkeyedScenes; i++)
            {

                EditorHotKeys hotKey = static_cast<EditorHotKeys>(EditorHotKeys::e_Scene_Select1 + i);
                if (s_EditorHotkeys[hotKey].IsActive())
                {
                    // Scenes::SetCurrentScene((int)i);
                }
            }
        }

		void local_Initialize()
		{
            Input::OnKey(TestOnKey);
            Input::OnMouse(TestOnMouse);
            Input::OnGamepad(TestOnGamepad);

            Settings::LoadUserSettings("Aaron.qpref"); // #TODO Set an expected default user/editor settings file to load or generate

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
            bool shouldOpenWelcomeWindow = Settings::GetUserSettings().showWelcomeWindow;
            for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
            {
                if (EditorWindowTypes::MenuBar == (u32)s_FocusedWindowsStack[i]->Type())
                {
                    // #TODO If the menu bar is missing, did the user close it? The menu should not be closable, or serializable really since it is forced
                    missingMenuBarWindow = false;
                    if (!shouldOpenWelcomeWindow)
                    {
                        break;
                    }
                }
                else if (EditorWindowTypes::WelcomeWindow == (u32)s_FocusedWindowsStack[i]->Type())
                {
                    shouldOpenWelcomeWindow = false;
                    if (!missingMenuBarWindow)
                    {
                        break;
                    }
                }
            }

            if (missingMenuBarWindow)
            {
                NewEditorWindow(EditorWindowTypes::MenuBar);
            }

            if (shouldOpenWelcomeWindow)
            {
                NewEditorWindow(EditorWindowTypes::WelcomeWindow);
            }

            // #TODO Move to Settings::Initialize()?
            const EngineSettings& engineSettings = Settings::GetEngineSettings();
            Time::SetMaximumFramerate(engineSettings.limitFramerate ? engineSettings.maxFramesPerSecond : engineSettings.maxAllowedFramesPerSecond);
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

        Input::InputMapping s_InputMapping = Input::InputMapping(std::vector<QKey>{ QKey::e_0, QKey::e_9, QKey::e_CtrlAny });
        bool wasActive = false;

        void local_Update()
        {
            if (s_ShowingEditorUI)
            {
                // #NOTE If using docking context, it must be submitted/drawn first before other windows, or else they will be undocked.
                // Any window drawn without a docking context become undocked, as described in this git issue: https://github.com/ocornut/imgui/issues/4757
                s_EditorWindowDockingContext.Draw();

                if (s_ShowingEditorUI)
                {
                    for (size_t i = 0; i < s_FocusedWindowsStack.size(); i++)
                    {
                        s_FocusedWindowsStack[i]->Draw(); // #TODO It's possible to add a new window during update, so a delayed ad to list is needed
                    }
                }
            }

            if (Input::KeyPressed(e_Any) || Input::KeyReleased(e_Any))
            {
                PollHotkeys(); // Update mapping IsActive states
                ResolveHotkeys(); // React to hotkey states
            }

            bool result = s_InputMapping.IsActive();
            if (result)
            {
                // LOG_INFO("Mapping IsActive()");
            }

            if (result != wasActive)
            {
                LOG_INFO("Mapping changed IsActive()");
            }
            wasActive = result;

            // if (Input::KeyDown(QKey::eKeys_A))
            {
                // LOG_INFO(Log::eLogLevel::Info, "KeyState");
            }
            if (Input::KeyPressed(QKey::e_Any))
            {
                // LOG_WARN("AnyPressed()");
            }
            if (Input::KeyReleased(QKey::e_Any))
            {
                // LOG_INFO("AnyReleased()");
            }
            if (Input::KeyDown(QKey::e_Any))
            {
                // LOG_WARN("AnyDown()");
            }
            if (Input::GamepadDown(QGamepad::e_GamepadAny))
            {
                // LOG_ERROR("Gamepad Any Down()");
            }
            if (Input::KeyPressed(QKey::e_AltAny))
            {
                // LOG_WARN("Any mod pressed");
            }
            if (Input::KeyReleased(QKey::e_AltAny))
            {
                // LOG_WARN("Any mod released");
            }

            vec2f axis;
            int axisIndex = 0;
            if (Input::GamepadAxisMoved(axisIndex))
            {
                axis = Input::GamepadAxis(axisIndex);
                // LOG_INFO("Gamepad Axis[{0}]: {1},{2}", axisIndex, axis.x, axis.y);
            }
            axisIndex += 2;
            if (Input::GamepadAxisMoved(axisIndex))
            {
                axis = Input::GamepadAxis(axisIndex);
                // LOG_WARN("Gamepad Axis[{0}]: {1},{2}", axisIndex, axis.x, axis.y);
            }
            axisIndex += 2;
            if (Input::GamepadAxisMoved(axisIndex))
            {
                axis = Input::GamepadAxis(axisIndex);
                // LOG_ERROR("Gamepad Axis[{0}]: {1},{2}", axisIndex, axis.x, axis.y);
            }

            if (Input::MouseScrolled())
            {
                float delta = Input::MouseScrollDelta();
                // LOG_ERROR("Mouse Scrolled: {0}", delta);
            }

            if (Input::MouseMoved())
            {
                // const vec2f mousePos = Input::MousePos();
                // LOG_INFO("Mouse pos: {0},{1}", mousePos.x, mousePos.y);

                // const vec2f delta = Input::MouseDelta();
                // LOG_INFO("Mouse delta: {0},{1}", delta.x, delta.y);
            }

            for (u8 i = 0; i < 14; i++)
            {
                if (Input::GamepadPressed(static_cast<QGamepad>(QGamepad::e_Gamepad0 + i)))
                // if (Input::GamepadPressed(QKey::e_Gamepad0 + i))
                {
                    // LOG_WARN("Pressed: {0}", i);
                }
                if (Input::GamepadReleased(static_cast<QGamepad>(QGamepad::e_Gamepad0 + i)))
                {
                    // LOG_ERROR("Released: {0}", i);
                }
            }

            // #TODO How to handle scrolling?
            // Expose mouse specific input, and should scrolling be abstracted away from just mice?
            if (Input::KeyPressed(QKey::e_ScrollDown))
            {
                // LOG_WARN("e_ScrollDownPressed()");
            }
            if (Input::KeyReleased(QKey::e_ScrollDown))
            {
                // LOG_WARN("e_ScrollDownReleased()");
            }
        }

        void local_EndFrame()
        {
            Framework::EndFrame();
            ++s_FramesCompleted;
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
