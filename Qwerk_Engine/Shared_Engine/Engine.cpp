#include "Engine.h"

#include "Engine_Enums.h"
#include "Editor/imgui_Editor.h"

#include "../QwerkE_Framework/Framework.h"

#include "../QwerkE_Framework/QwerkE_Common/Libraries/glew/GL/glew.h"
#include "../QwerkE_Framework/QwerkE_Common/Libraries/glfw/GLFW/glfw3.h"
#include "../QwerkE_Framework/QwerkE_Common/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/QwerkE_Common/Libraries/imgui/imgui_impl_glfw.h"
#include "../QwerkE_Framework/QwerkE_Common/Libraries/imgui/imgui_impl_opengl3.h"

#include "../QwerkE_Framework/QwerkE_Common/Utilities/Helpers.h"
#include "../QwerkE_Framework/QwerkE_Common/Utilities/ProgramArgs.h"

#include "../QwerkE_Framework/Graphics/Graphics_Header.h"
#include "../QwerkE_Framework/Graphics/FrameBufferObject.h"
#include "../QwerkE_Framework/Graphics/Mesh/MeshFactory.h"

#include "../QwerkE_Framework/Systems/Input/Input.h"
#include "../QwerkE_Framework/Systems/Resources/Resources.h"
#include "../QwerkE_Framework/Systems/Services.h"
#include "../QwerkE_Framework/Systems/Events/EventManager.h"
#include "../QwerkE_Framework/Systems/Scenes.h"
#include "../QwerkE_Framework/Systems/Factory/Factory.h"
#include "../QwerkE_Framework/Systems/Window/CallbackFunctions.h"
#include "../QwerkE_Framework/Systems/Physics/Physics.h"
#include "../QwerkE_Framework/Systems/Renderer/Renderer.h"
#include "../QwerkE_Framework/Systems/MessageManager.h"
#include "../QwerkE_Framework/Systems/Audio/Audio.h"
#include "../QwerkE_Framework/Systems/Debugger/Debugger.h"
#include "../QwerkE_Framework/Systems/ShaderFactory/ShaderFactory.h"
#include "../QwerkE_Framework/Systems/Jobs/Jobs.h"
#include "../QwerkE_Framework/Systems/Network/Network.h"
#include "../QwerkE_Framework/Systems/Window/Window.h"
#include "../QwerkE_Framework/Systems/Window/WindowManager.h"
#include "../QwerkE_Framework/Systems/Window/glfw_Window.h"

#include "../QwerkE_Framework/Modules/Time.h"

namespace QwerkE {

    // private engine variables
    static WindowManager* m_WindowManager = nullptr;
    static Scenes* m_Scenes = nullptr;
    static bool m_IsRunning = false;
    static Editor* m_Editor = nullptr;

    struct FrameTimer
    {
        double timeSinceLastFrame = 0.0;
        float frameRate = 0.0f;
        double deltaTime = 0.0f; // Time between current frame and last frame
        double lastFrame = 0.0f; // Time of last frame initialized to current time

        // Limit framerate
        int FPS_MAX = 120; // maximum number of frames that can be run be second
        float FPS_MAX_DELTA = 1.0f / FPS_MAX;

        // timeSinceLastFrame = FPS_MAX; // Amount of seconds since the last frame ran initialized to run 1st time
        // Printing framerate
        float printPeriod = 3.0f; // Print period in seconds
        float timeSincePrint = 0.0f; // Seconds since last print initialized to print 1st frame
        short framesSincePrint = 0;

        FrameTimer() {}
        FrameTimer(int maxFPS, float printPeriod)
        {
            FPS_MAX = maxFPS;
            timeSincePrint = printPeriod;
            FPS_MAX_DELTA = 1.0f / FPS_MAX;
            timeSinceLastFrame = FPS_MAX;
        }
        ~FrameTimer() {}
    };

	namespace Engine
    {
        void DockingSetup();

		void Engine::Run(std::map<const char*, const char*> &args)
		{
			// Handle program arguments
			if (args.find(key_ProjectName) != args.end())
			{
				// Load project (reusable method)
				// Could find and save preferences file path
			}

			// TODO: check if(initialized) in case user defined simple API.
			// Might want to create another function for the game loop and
			// leave Run() smaller and abstracted from the functionality.

			std::uint_fast8_t flags = 0;

			// TEMP: Turn off components like this until the data can be read from a file
			flags &= ~Flag_Physics;
			flags &= ~Flag_Renderer;
			flags &= ~Flag_Audio;

			if (Framework::Startup(ConfigsFolderPath("preferences.qpref"), flags) == eEngineMessage::_QFailure)
			{
				ConsolePrint("\nQwerk Framework failed to load! Shutting down engine.\n");
				return;
			}

			Services::LockServices(true); // prevent service changes

			Scenes::GetCurrentScene()->SetIsEnabled(true); // enable default scene

			m_WindowManager = (WindowManager*)Services::GetService(eEngineServices::WindowManager);

			m_IsRunning = true;

			// Delta time
			FrameTimer fps(120, 3.0f);
			fps.lastFrame = helpers_Time(); // Time of last frame initialized to current time

			Time::SetDeltatime(&fps.timeSinceLastFrame);
			Time::SetFrameRate(&fps.frameRate);

			// TODO: GL state init should be in a Window() or OpenGLManager()
			// class or some type of ::Graphics() system.
			glClearColor(0.5f, 0.7f, 0.7f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// turn on depth buffer testing
			glEnable(GL_DEPTH_TEST);
			glPointSize(10);
			glLineWidth(10);

			// depth cull for efficiency
			// TODO: This is also in the framework
			// glEnable(GL_CULL_FACE);
			// glDisable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			if (Wind_CCW) glFrontFace(GL_CCW);
			else glFrontFace(GL_CW);

			// turn on alpha blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glViewport(0, 0, g_WindowWidth, g_WindowHeight);

#ifdef dearimgui
			m_Editor = (Editor*)new imgui_Editor();
#else
			m_Editor = (Editor*)new ????_Editor();
#endif // editor

			// Application Loop
			while (m_IsRunning) // Run until close requested
			{
				// setup frame
				// Calculate deltatime of current frame
				double currentFrame = helpers_Time();
				fps.deltaTime = currentFrame - fps.lastFrame; // time since last frame
				fps.lastFrame = currentFrame; // save last frame

				// FPS display + tracking
				if (fps.timeSincePrint >= fps.printPeriod) // print period
				{
					fps.frameRate = 1.0f / fps.timeSincePrint * fps.framesSincePrint;
					// OutputPrint("\nFPS: %f", frameRate); // FPS printout
					// OutputPrint("\nFrames: %i", framesSincePrint); // Frames printout
					fps.timeSincePrint = 0.0f;
					fps.framesSincePrint = 0;
				}

				fps.timeSincePrint += (float)fps.deltaTime;
				fps.timeSinceLastFrame += fps.deltaTime;

				/* Application Loop */
				if (fps.timeSinceLastFrame >= fps.FPS_MAX_DELTA) // Run frame?
				{
					/* New Frame */
					Engine::NewFrame();

					/* Input */
					Engine::PollInput();

					/* Logic */
					Engine::Update(fps.timeSinceLastFrame);

					/* Render */
					Engine::Draw();

					// FPS
					fps.framesSincePrint++; // Framerate tracking
					fps.timeSinceLastFrame = 0.0; // FPS_Max
				}
				// else
				{
					// Skip frame
					YieldProcessor;
				}
			}

			// unlock services for clean up
			Services::LockServices(false);

			Framework::TearDown();
		}

		void Engine::Stop()
		{
			m_IsRunning = false;
		}

		void Engine::NewFrame() /* Reset */
		{
			Framework::NewFrame();
            m_Editor->NewFrame();
			// NOTE: ImGUI::NewFrame() is in Framework::PollInput()!
		}

		void Engine::PollInput()
		{
			Framework::PollInput();
		}

		void Engine::Update(double deltatime)
		{
			m_Scenes->Update(deltatime);
			m_Editor->Update();

			// Framework::Update();

			if (Input::FrameKeyAction(eKeys::eKeys_P, eKeyState::eKeyState_Press)) // pause entire scene
			{
				static bool paused = false;
				paused = !paused;
				if (paused)
				{
					m_Scenes->GetCurrentScene()->SetState(eSceneState::SceneState_Paused);
				}
				else
				{
					m_Scenes->GetCurrentScene()->SetState(eSceneState::SceneState_Running);
				}
			}
			if (Input::FrameKeyAction(eKeys::eKeys_Z, eKeyState::eKeyState_Press))// pause actor updates
			{
				static bool frozen = false;
				frozen = !frozen;
				if (frozen)
				{
					m_Scenes->GetCurrentScene()->SetState(eSceneState::SceneState_Frozen);
				}
				else
				{
					m_Scenes->GetCurrentScene()->SetState(eSceneState::SceneState_Running);
				}
			}
			if (Input::FrameKeyAction(eKeys::eKeys_F, eKeyState::eKeyState_Press))
			{
				m_Editor->ToggleFullScreenScene();
			}
			if (Input::FrameKeyAction(eKeys::eKeys_Escape, eKeyState::eKeyState_Press))
			{
				WindowManager* windowManager = (WindowManager*)Services::GetService(eEngineServices::WindowManager);
				m_WindowManager->GetWindow(0)->SetClosing(true); // close glfw
				Framework::Stop();
				Engine::Stop();
			}
		}

		void Engine::Draw()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // new frame

			DockingSetup();

            m_Editor->Draw();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }

			m_WindowManager->GetWindow(0)->SwapBuffers();
		}

		bool Engine::StillRunning()
		{
			return m_IsRunning;
		}

		void DockingSetup()
		{
			// TODO: Move this code to a better place
            static bool opt_fullscreen_persistant = true;
            bool opt_fullscreen = opt_fullscreen_persistant;

            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
            static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

            if (opt_fullscreen) // fullscreen
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->Pos);
                ImGui::SetNextWindowSize(viewport->Size);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }

            static bool open = true;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", &open, window_flags);
            ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("QwerkEDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            static bool dockspaceOpen = true;

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Docking"))
                {
                    // Disabling fullscreen would allow the window to be moved to the front of other windows,
                    // which we can't undo at the moment without finer window depth/z control.
                    //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                    if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
                    if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
                    if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
                    if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))     dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
                    if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
                    ImGui::Separator();
                    if (ImGui::MenuItem("Close DockSpace", NULL, false, dockspaceOpen != NULL))
                        dockspaceOpen = false;
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            ImGui::End();
		}
	}
}
