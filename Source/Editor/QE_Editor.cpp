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
        static bool s_ShowingWindowStackPanel = false; // #TODO Move logic to its own window or editor file
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

        void local_FileDropCallback(const char* filePath);

        bool s_ReloadRequested = false; // #TODO FEATURE

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

                    Renderer::StartImGui();

                    if (s_ShowingEditorUI)
                    {
                        s_EditorWindowDockingContext.Draw();
                    }

                    local_Update();

					Framework::Update((float)Time::PreviousFrameDuration());

                    Renderer::EndImGui();

                    local_EndFrame();
                    // #TODO Review Framework::EndFrame();
				}
				else
				{
                    // #TODO Review sleep function
                    // From: https://github.com/ocornut/imgui/blob/docking/backends/imgui_impl_glfw.cpp#L1006
#ifdef _WIN32
					YieldProcessor();
                    // ::Sleep(milliseconds);
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
                // #TODO Consider reversing direction and storing recently interacted windows at the back of the vector.
                // This would avoid swapping the entire list for new windows (just push instead) and deleted windows probably had focus anyways (just pop the top).
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
            // #TODO Consider moving to a static function in EditorWindow.h
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

        void OnKey(QKey a_Key, QKeyState a_State)
        {
            LOG_INFO("OnKey(): {0}, {1}", a_Key, a_State);
            return;

            if (QKey::e_1 == a_Key)
            {
                LOG_INFO("OnKey(): {0}, {1}", a_Key, a_State);
                Input::OnKeyStop(OnKey);
            }
        }

        void OnMouse(QKey a_Key, QKeyState a_State, float a_ScrollDelta, const vec2f& a_MousePosition)
        {
            if (QwerkE::e_MouseMove != a_Key)
            {
                LOG_INFO("OnMouse(): {0}, {1}, {2}, {3}, {4}", a_Key, a_State, a_ScrollDelta, a_MousePosition.x, a_MousePosition.y);
                return;
            }

            switch (a_Key)
            {
            case QwerkE::e_MouseLeft:
                LOG_INFO("OnMouse(): {0}, {1}, {2}, {3}, {4}", a_Key, a_State, a_ScrollDelta, a_MousePosition.x, a_MousePosition.y);
                // Input::OnMouseStop(OnMouse);
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
                LOG_INFO("OnMouse(): {0}, {1}, {2}, {3}, {4}", a_Key, a_State, a_ScrollDelta, a_MousePosition.x, a_MousePosition.y);
                break;
            case QwerkE::e_ScrollDown:
                LOG_INFO("OnMouse(): {0}, {1}, {2}, {3}, {4}", a_Key, a_State, a_ScrollDelta, a_MousePosition.x, a_MousePosition.y);
                break;
            case QwerkE::e_MouseMove:
                // LOG_INFO("OnMouse(): {0}, {1}, {2}, {3}, {4}", a_Key, a_State, a_ScrollDelta, a_MousePosition.x, a_MousePosition.y);
                break;
            }
        }

        void OnGamepad(QGamepad a_Input, QKeyState a_State, const vec2f& a_Axis12, const vec2f& a_Axis34, const vec2f& a_Axis56, QGamepad a_GamepadId)
        {
            switch (a_Input)
            {
            case QwerkE::e_GamepadA:
                LOG_INFO("OnGamepad(): {0}, {1}", a_Input, a_State, a_Axis12.x, a_Axis12.y, a_Axis34.x, a_Axis34.y, a_Axis56.x, a_Axis56.y, a_GamepadId);
                // Input::OnGamepadStop(s_OnGamepadGuid);
                Input::OnGamepadStop(OnGamepad);
                break;
            case QwerkE::e_GamepadAxis01:
                LOG_INFO("OnGamepad(): {0}, {1}", a_Input, a_State, a_Axis12.x, a_Axis12.y, a_Axis34.x, a_Axis34.y, a_Axis56.x, a_Axis56.y, a_GamepadId);
                break;
            }
        }

		void local_Initialize()
		{
            Input::OnKey(OnKey);
            Input::OnMouse(OnMouse);
            Input::OnGamepad(OnGamepad);

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

        void local_Update()
        {
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

#ifdef _QDEBUG
            Debug::DrawCube({}, 1.f, false, Debug::g_Purple);
#endif // _QDEBUG

            if (Input::KeyPressed(QKey::e_Escape))
            {
                Window::RequestClose();
            }

            if (!s_ShowingWindowStackPanel)
            {
                if (Input::KeyPressed(QKey::e_Tab) && Input::KeyDown(QKey::e_CtrlAny))
                {
                    s_ShowingWindowStackPanel = true;
                }
            }
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

            if (Input::KeyPressed(QKey::e_R) && Input::KeyDown(QKey::e_CtrlAny))
            {
                RequestRestart();
            }

            if (Input::KeyPressed(QKey::e_U) && Input::KeyDown(QKey::e_CtrlAny))
            {
                s_ShowingEditorUI = !s_ShowingEditorUI;
            }

            constexpr size_t numberOfHotkeyedScenes = QKey::e_F12 - QKey::e_F1 + 1;
            for (u8 i = 0; i < numberOfHotkeyedScenes; i++)
            {
                QKey qKey = static_cast<QKey>(QKey::e_F1 + i);
                if (Input::KeyPressed(qKey))
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

            if (constexpr bool notificationsEnabled = true)
            {
                // Render toasts on top of everything, at the end of your code!
                // You should push style vars here
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
                ImGui::RenderNotifications();
                ImGui::PopStyleVar(1); // Don't forget to Pop()
                ImGui::PopStyleColor(1);
            }
        }

        void local_EndFrame()
        {
#ifdef _QDEBUG
            if (!Window::IsMinimized())
            {
                // #TODO Replace editor draws by moving to EditorWindowSceneView class
                Debug::DrawSphere(vec3f(.0f, .0f, .0f), 0.1f);
                Debug::DrawGrid(vec3f(.0f, .0f, .0f), 50);
            }
#endif // _QDEBUG
            Framework::EndFrame();
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
