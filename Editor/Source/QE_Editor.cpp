#include "QE_Editor.h"

#include <map>      // For std::map<std::string, const char*> pairs;
#include <string>   // For std::map<std::string, const char*> pairs;

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
#include "QC_Time.h"

#include "QF_Assets.h"
#include "QF_Directory.h"
#include "QF_Framework.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Mesh.h"
#include "QF_Projects.h"
#include "QF_Renderer.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Serialization.h"
#include "QF_Settings.h"
#include "QF_Shader.h"
#include "QF_Window.h"

#include "QE_ProgramArgs.h"

// #TODO Remove after supporting EditorWindow classes
#include "QE_EditorInspector.h"

// #TODO Remove after adding indirection
#include "QE_EditorWindowAssets.h"
#include "QE_EditorWindowDockingContext.h"
#include "QE_EditorWindowEntityInspector.h"
#include "QE_EditorWindowImGuiDemo.h"
#include "QE_EditorWindowMenuBar.h"
#include "QE_EditorWindowSceneControls.h"
#include "QE_EditorWindowSceneGraph.h"
#include "QE_EditorWindowSceneView.h"
#include "QE_EditorWindowSettings.h"
#include "QE_EditorWindowStylePicker.h"
#include "QE_EditorWindowDefaultDebug.h"

namespace QwerkE {

    struct EditorCamera
    {
        void PreDrawSetup(bgfx::ViewId viewId)
        {
            const vec2f& windowSize = Window::GetSize();

            bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
            bgfx::setViewRect(viewId, 0, 0, uint16_t(windowSize.x), uint16_t(windowSize.y));

            bx::mtxLookAt(m_View, m_Eye, m_LookAtTarget);

            bx::mtxProj(m_Proj, m_Fov, windowSize.x / windowSize.y, m_Near, m_Far, bgfx::getCaps()->homogeneousDepth);
            bgfx::setViewTransform(viewId, m_View, m_Proj);

            if (m_ShowSphere)
            {
                DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer();
                debugDrawer.begin(viewId);
                debugDrawer.drawSphere(m_LookAtTarget.x, m_LookAtTarget.y, m_LookAtTarget.z, 3.f);
                debugDrawer.end();
            }
        }

        bool m_ShowSphere = false;
        bx::Vec3 m_Eye = bx::Vec3(0.f);
        bx::Vec3 m_LookAtTarget = bx::Vec3(0.f);;

        float m_Fov = 60.f;
        float m_Near = .1f;
        float m_Far = 100.f;

        float m_View[16];
        float m_Proj[16];
    };

    void LoadImGuiStyleFromFile() // #TODO Move somewhere else
    {
        ImGuiStyle& style = ImGui::GetStyle();
        Serialization::DeserializeObjectFromFile(Settings::GetStyleFileName(), style);
    }

	namespace Editor {

        static bool s_ShowingEditorUI = true;

        // #TODO Move windows to a collection
        static EditorWindowAssets s_EditorWindowAssets;
        static EditorWindowDefaultDebug s_EditorWindowDefaultDebug;
        static EditorWindowDockingContext s_EditorWindowDockingContext;
        static EditorWindowEntityInspector s_EditorWindowEntityInspector;
        static EditorWindowImGuiDemo s_EditorWindowImGuiDemo;
        static EditorWindowMenuBar s_EditorWindowMenuBar;
        static EditorWindowSceneControls s_EditorWindowSceneControls;
        static EditorWindowSceneGraph s_EditorWindowSceneGraph;
        static EditorWindowSceneView s_EditorWindowSceneViewEditor("EditorView");
        static EditorWindowSceneView s_EditorWindowSceneViewGame("GameView");
        static EditorWindowSettings s_EditorWindowSettings;
        static EditorWindowStylePicker s_EditorWindowStylePicker;

        static EditorCamera s_EditorCamera;

        static EditorStateFlags s_EditorStateFlags = EditorStateFlags::EditorStateFlagsNone;

        void local_EditorInitialize();
        void local_Shutdown();
        void local_EndFrame();

		void Run(unsigned int argc, char** argv)
		{
            Log::Console("-- Qwerk Editor %s --\n", std::to_string(QWERKE_VERSION).c_str());

            std::map<std::string, const char*> pairs;
            ProgramArgsToPairs(argc, argv, pairs);

            if (pairs.find(key_ApplicationName) == pairs.end())
            {
                pairs.insert(std::pair<const char*, const char*>(key_ApplicationName, EngineName));
            }

            if (pairs.find(key_ProjectFileName) == pairs.end())
            {
                pairs.insert(std::pair<const char*, const char*>(key_ProjectFileName, "Project1"));
            }
            else
            {
                // #TODO Load last opened project
            }

            if (pairs.find(key_UserName) == pairs.end())
            {
                pairs.insert(std::pair<const char*, const char*>(key_UserName, "User1"));
            }

            pairs.insert(std::pair<const char*, const char*>("WorkspaceRootDir", WorkspaceRootDir));

            if (true) { OutputProgramPairsInfo(pairs); }

			Framework::Initialize();

            local_EditorInitialize();
            LoadImGuiStyleFromFile();

            Scenes::Initialize();

            s_EditorWindowSceneViewEditor.SetViewId(3);
            s_EditorWindowSceneViewGame.SetViewId(4);

            const EngineSettings& engineSettings = Settings::GetEngineSettings();
            Time::SetMaximumFramerate(engineSettings.limitFramerate ? engineSettings.maxFramesPerSecond : engineSettings.defaultMaxFramesPerSecond);
            Time::WriteAppStartTime();

			while (StillRunning())
			{
				if (Time::ShouldProcessNextFrame())
				{
                    Time::StartFrame();

					Framework::StartFrame();

					if (Input::FrameKeyAction(eKeys::eKeys_Escape, eKeyState::eKeyState_Press))
					{
						Stop();
					}
                    if (Input::FrameKeyAction(eKeys::eKeys_U, eKeyState::eKeyState_Press) &&
                        (Input::IsKeyDown(eKeys::eKeys_LCTRL) || Input::IsKeyDown(eKeys::eKeys_RCTRL)))
                    {
                        s_ShowingEditorUI = !s_ShowingEditorUI;
                    }

                    Renderer::StartImGui();

                    s_EditorWindowDockingContext.Draw();
                    s_EditorWindowMenuBar.Draw();
                    s_EditorWindowDefaultDebug.Draw();

                    if (true)
                    {
                        constexpr size_t numberOfHotkeyedScenes = eKeys::eKeys_F12 - eKeys::eKeys_F1 + 1;
                        for (size_t i = 0; i < numberOfHotkeyedScenes; i++)
                        {
                            if (Input::FrameKeyAction((eKeys)(eKeys::eKeys_F1 + i), eKeyState::eKeyState_Press))
                            {
                                Scenes::SetCurrentScene((int)i);
                                break;
                            }
                        }
                    }

					Framework::Update((float)Time::PreviousFrameDuration());

                    s_EditorWindowAssets.Draw();
                    s_EditorWindowSceneViewEditor.Draw();
                    s_EditorWindowSceneViewGame.Draw();
                    s_EditorWindowSettings.Draw();
                    s_EditorWindowStylePicker.Draw();
                    s_EditorWindowSceneControls.Draw();
                    s_EditorWindowSceneGraph.Draw();
                    s_EditorWindowEntityInspector.Draw();
                    s_EditorWindowImGuiDemo.Draw();

                    Renderer::EndImGui();

                    local_EndFrame();
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

		void ResetReferences()
		{
            // #TODO Add callbacks to EditorWindows to register to, so they know whn things happen like :
            // Selection made, references are dirty (state change)
			// s_EntityEditor->ResetReferences();
		}

        const EditorStateFlags& GetEditorStateFlags()
        {
            return s_EditorStateFlags;
        }

        void SetEditorStateFlags(const EditorStateFlags& flags)
        {
            s_EditorStateFlags = (EditorStateFlags)(s_EditorStateFlags ^ flags);
        }

        bool ShowingEditorUI()
        {
            return s_ShowingEditorUI;
        }

		void local_EditorInitialize()
		{
            // #TODO Add instances of EditorWindow sub classes to a collection.
            // Could load previous editor state to reload window info (sizes, positions, selections, etc)
		}

		void local_Shutdown()
		{
            // #TODO Delete EditorWindow instances
		}

        void local_EditorDrawImGui(bool showEditorUI)
        {
            if (Window::IsMinimized() || !showEditorUI)
                return;
        }

        void local_EndFrame()
        {
            if (Window::IsMinimized())
                return;

            const bgfx::ViewId viewIdFbo1 = 2; // #TODO Fix hard coded value
            {   // Debug drawer calls
                DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer(); // #TESTING
                debugDrawer.begin(viewIdFbo1, true);

                const bx::Vec3 normal = { .0f,  1.f, .0f };
                const bx::Vec3 pos = { .0f, .0f, .0f };

                debugDrawer.drawSphere(0.f, 0.f, 0.f, 3.f, Axis::X);

                bx::Plane plane(bx::InitNone);
                bx::calcPlane(plane, normal, pos);

                debugDrawer.drawGrid(Axis::Y, pos, 50, 1.0f);

                debugDrawer.end();
            }

            Framework::RenderView(viewIdFbo1);

            s_EditorCamera.PreDrawSetup(0);
            Framework::RenderView(0);

            Framework::EndFrame();
        }

	}

}
