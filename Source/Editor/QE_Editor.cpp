#include "QE_Editor.h"

#include <map>      // For std::map<std::string, const char*> pairs; and EditorWindows collection
#include <string>   // For std::map<std::string, const char*> pairs;
#include <typeinfo> // For typeid()
#include <vector>   // For s_EditorWindowsQueuedForDelete

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

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
#include "QF_Window.h"

#include "QE_EditorWindowHelpers.h"
#include "QE_Projects.h"
#include "QE_Settings.h"

namespace QwerkE {

    void LoadImGuiStyleFromFile() // #TODO Move somewhere else
    {
        ImGuiStyle& style = ImGui::GetStyle();
        Serialize::FromFile(Paths::Setting(Settings::GetStyleFileName()).c_str(), style, true);
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

        void local_FileDropCallback(const char* filePath);

        bool s_ReloadRequested = false; // #TODO FEATURE

        // #TODO Change to main(argc, argv) entry point function
		void Run(unsigned int argc, char** argv)
		{
            Log::Console("-- Qwerk Editor %s --\n", std::to_string(QWERKE_VERSION).c_str());

            // #TODO Note order dependency with framework paths needing initialization
            const EngineSettings& engineSettings = Settings::GetEngineSettings();

            Framework::StartUpArguments startUpArgs(
                argc,
                argv,
                engineSettings.windowWidthPixels,
                engineSettings.windowHeightPixels
            );

			Framework::Initialize(startUpArgs);

            local_Initialize();

            FixMatrixSerialize matrix;
            Serialize::ToFile(matrix, Paths::Setting("test_matrix.json").c_str());

            Time::WriteAppStartTime();

			while (StillRunning())
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
					YieldProcessor();
				}
			}

			Settings::SaveEngineSettings();

            // #TODO Save editor state for next launch

			Framework::Shutdown();
            local_Shutdown();
		}

		void Stop()
		{
			Window::RequestClose();
		}

		bool StillRunning()
		{
			return !Window::CloseRequested();
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

		void local_Initialize()
		{
            Projects::Initialize();

            // #TODO Register file drop callback
            // Window::RegisterFileDropCallback(&local_FileDropCallback);

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
                Stop();
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

void main(unsigned int argc, char** argv)
{
    QwerkE::Editor::Run(argc, argv);
}
