#include "Engine.h"

#include "Headers/Engine_Enums.h"

// TODO: Properly support editor library switching
#define IMGUI_EDITOR 1
#if 1
#include "Editor/imgui_Editor/imgui_Editor.h"
#endif // 0

// #include "../../QwerkE_Framework/QwerkE.h"

#include "../../QwerkE_Framework/Source/Framework.h"

#include "../../QwerkE_Framework/Libraries/glew/GL/glew.h"
#include "../../QwerkE_Framework/Libraries/glfw/GLFW/glfw3.h"
#include "../../QwerkE_Framework/Libraries/imgui/imgui.h"
#include "../../QwerkE_Framework/Libraries/imgui/imgui_impl_glfw.h"
#include "../../QwerkE_Framework/Libraries/imgui/imgui_impl_opengl3.h"

#include "../../QwerkE_Framework/Source/Utilities/Helpers.h"
#include "../../QwerkE_Framework/Source/Utilities/ProgramArgs.h"

#include "../../QwerkE_Framework/Source/Core/Graphics/Graphics_Header.h"
#include "../../QwerkE_Framework/Source/Core/Graphics/DataTypes/FrameBufferObject.h"
#include "../../QwerkE_Framework/Source/Core/Graphics/Mesh/MeshFactory.h"
#include "../../QwerkE_Framework/Source/Core/Graphics/Renderer.h"

#include "../../QwerkE_Framework/Source/Debug/Profiler/Profiler.h"
#include "../../QwerkE_Framework/Source/Debug/Debugger/Debugger.h"
#include "../../QwerkE_Framework/Source/Core/Input/Input.h"
#include "../../QwerkE_Framework/Source/Core/Resources/Resources.h"
#include "../../QwerkE_Framework/Source/Core/Events/EventManager.h"
#include "../../QwerkE_Framework/Source/Core/Scenes/Scene.h"
#include "../../QwerkE_Framework/Source/Core/Window/CallbackFunctions.h"
#include "../../QwerkE_Framework/Source/Core/Physics/Physics.h"
#include "../../QwerkE_Framework/Source/Core/Audio/Audio.h"
#include "../../QwerkE_Framework/Source/Core/Graphics/ShaderFactory/ShaderFactory.h"
#include "../../QwerkE_Framework/Source/Core/Jobs/Jobs.h"
#include "../../QwerkE_Framework/Source/Core/Network/Network.h"
#include "../../QwerkE_Framework/Source/Core/Window/Window.h"
#include "../../QwerkE_Framework/Source/Core/Window/Windows.h"
#include "../../QwerkE_Framework/Source/Core/Window/glfw_Window.h"
#include "../../QwerkE_Framework/Source/Core/Time/Time.h"

namespace QwerkE {

	namespace Engine
    {
        // Private engine variables
        static bool m_IsRunning = false;
        static Editor* m_Editor = nullptr;

		void Engine::Run(std::map<const char*, const char*> &args)
        {
            Instrumentor::Get().BeginSession("Instrumentor", "instrumentor_log.json");
			PROFILE_SCOPE("Run");

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
                Log::Safe("Qwerk Framework failed to load. Shutting down engine.");
				return;
			}

			Scenes::GetCurrentScene()->SetIsEnabled(true); // enable default scene

			m_IsRunning = true;

			// Delta time
			//FrameTimer fps(120, 3.0f);
			//fps.lastFrame = Time::Now(); // Time of last frame initialized to current time

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

			// TODO: Move to better file
#ifdef dearimgui
			m_Editor = (Editor*)new imgui_Editor();
#else
			m_Editor = (Editor*)new ????_Editor();
#endif // editor

			// TODO: Move this to a window class
			const unsigned char FPS_MAX = 144;
			const double FPS_MAX_DELTA = 1.0 / FPS_MAX;

            /* Application Loop */
			double deltaTime = 0.0;
			double elapsedTime = 0.0;

			while (m_IsRunning)
			{
                Time::NewFrame();

				deltaTime = Time::Delta();
				elapsedTime += deltaTime;
				// if (elapsedTime >= FPS_MAX_DELTA)
				{
					/* New Frame */
					Engine::NewFrame();

					/* Input */
					Engine::PollInput();

					/* Logic */
					Engine::Update(elapsedTime);

					/* Render */
                    Engine::Draw();

                    elapsedTime = 0.0;
				}
				// TODO : Fix yield that broke after deltatime changes
                // else
                // {
                // 	YieldProcessor(); // TODO: Look into proper yield behaviour
                // }
			}

            Instrumentor::Get().EndSession();
			Framework::TearDown();
		}

		void Engine::Stop()
		{
			m_IsRunning = false;
		}

		void Engine::NewFrame()
		{
			Framework::NewFrame();
            m_Editor->NewFrame();
			// NOTE: ImGUI::NewFrame() is in Framework::PollInput()!
		}

		void Engine::PollInput()
        {
            PROFILE_SCOPE("Engine Input");
			Framework::PollInput();
		}

		void Engine::Update(double deltatime)
        {
            PROFILE_SCOPE("Engine Update");

			Physics::Tick();

			Scenes::Update(deltatime);
			m_Editor->Update();

			// Framework::Update();

			// TODO: Move this behaviour into Scenes for scene specific behaviour managed by the scene manager
			if (Input::FrameKeyAction(eKeys::eKeys_P, eKeyState::eKeyState_Press)) // pause entire scene
			{
				static bool paused = false;
				paused = !paused;
				if (paused)
				{
					Scenes::GetCurrentScene()->SetState(eSceneState::SceneState_Paused);
				}
				else
				{
					Scenes::GetCurrentScene()->SetState(eSceneState::SceneState_Running);
				}
			}

			if (Input::FrameKeyAction(eKeys::eKeys_Z, eKeyState::eKeyState_Press))// pause actor updates
			{
				static bool frozen = false;
				frozen = !frozen;
				if (frozen)
				{
					Scenes::GetCurrentScene()->SetState(eSceneState::SceneState_Frozen);
				}
				else
				{
					Scenes::GetCurrentScene()->SetState(eSceneState::SceneState_Running);
				}
			}

			if (Input::FrameKeyAction(eKeys::eKeys_F, eKeyState::eKeyState_Press))
			{
				m_Editor->ToggleFullScreenScene();
			}

			if (Input::FrameKeyAction(eKeys::eKeys_Escape, eKeyState::eKeyState_Press))
			{
				Windows::GetWindow(0)->SetClosing(true);
				Framework::Stop();
				Engine::Stop();
			}
		}

		void Engine::Draw()
        {
			// TODO: Move library calls into an abstracted class like Window, etc
            PROFILE_SCOPE("Engine Render");
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

			Windows::GetWindow(0)->SwapBuffers();
		}

		bool Engine::StillRunning()
		{
			return m_IsRunning;
		}
	}
}
