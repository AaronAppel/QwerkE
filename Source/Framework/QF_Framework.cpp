#include "QF_Framework.h"

#include "QF_QwerkE_Defines.h"

// #ifdef GLFW3 // #TODO Move library dependent logic into appropriate class, like Window
#include "Libraries/glew/GL/glew.h"
#include "Libraries/glfw/GLFW/glfw3.h"
#pragma warning( disable : 4244 )
#include "Libraries/imgui/imgui.h"
#include "Libraries/imgui/imgui_impl_glfw.h"
#include "Libraries/imgui/imgui_impl_opengl3.h"
#pragma warning( default : 4244 )
// #endif

#include "QF_QwerkE_Enums.h"
#include "QF_Libraries_Initialize.h"

#include "QC_Helpers.h"

#include "QF_Audio.h"
#include "QF_ConfigHelper.h"
#include "QF_DataManager.h"
#include "QF_EventManager.h"
#include "QF_Factory.h"
#include "QF_MeshFactory.h"
#include "QF_Graphics_Header.h"
#include "QF_Renderer.h"
#include "QF_ShaderFactory.h"
#include "QF_Input.h"
#include "QF_Jobs.h"
#include "QF_Network.h"
#include "QF_Physics.h"
#include "QF_Resources.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Time.h"
#include "QF_Window.h"
#include "QF_Windows.h"
#include "QF_glfw_Window.h"
#include "QF_CallbackFunctions.h"
#include "QF_Debugger.h"
#include "QF_Log.h"
#include "QF_FileSystem.h"

namespace QwerkE {

    extern const char* g_WindowTitle = "QwerkEngine";

    static Window* m_Window = nullptr;
    static bool m_IsRunning = false;

	namespace Framework
	{
		eEngineMessage Framework::Startup()
		{
			return Startup(ConfigsFolderPath("preferences.qpref"));
		}

		eEngineMessage Framework::Startup(const std::string configFilePath)
		{
            Log::Initialize();

            ConfigHelper::LoadConfigData(configFilePath); // Init config data
            const ConfigData& config = ConfigHelper::GetConfigData();

            // #TODO Load libraries dynamically. Need functions to load .dlls

			if (!Libs_Setup())
			{
				LOG_CRITICAL("{0} Error loading libraries!", __FUNCTION__);
				return eEngineMessage::_QFailure;
            }

            // #TODO Cleanup switch or if/else if statements below. Find a nice way to detect which library objects to load

			// #TODO Try to reduce or avoid order dependency in system creation

			// Load and register systems
			// Audio, Networking, Graphics (Renderer, GUI), Utilities (Conversion, FileIO, Printing),
            // Physics, Event, Debug, Memory, Window, Application, Input, Resources

			// ShaderFactory // #Dependency Resources

			// #TODO Could change to data and iterate over systems in a loop.
			//	Each system has a name string (JSON parent object name) and enabled boolean.
			//	If false, skip, true loads and on error/success print system name string.

			if (config.libraries.Window == "GLFW3") // #TODO Improve platform handling
			{
#ifdef GLFW3
				m_Window = new glfw_Window(Renderer::g_WindowWidth, Renderer::g_WindowHeight, g_WindowTitle);
				LOG_TRACE("Window created successfully");
#endif
			}
            else
            {
				LOG_ERROR("No window library detected! Check config libraries value."); // #TODO Review handling missing library declaration
                assert(false); // #TODO Implement and swap to a QwerkE assert
            }

            Windows::AddWindow(m_Window);

            Input::Initialize((GLFWwindow*)Windows::GetWindow(0)->GetContext()); // #TODO Remove glfw code

            if (config.systems.AudioEnabled && Audio::Initialize())
            {
                LOG_TRACE("Audio system initialized with OpenAL.");
            }
            else
            {
				LOG_WARN("No audio system loaded.");
            }

            Resources::Initialize(); // #Dependencies Audio, OpenGL, Window?

			Renderer::Initialize();
			Renderer::DrawFont("Loading...", 300.f, 100.f, 5.0f);
			m_Window->SwapBuffers();

			EventManager::Initialize(); // #TODO Set max thread count with config.framework.MaxConcurrentThreadCount;

			if (config.systems.NetworkingEnabled)
			{
				Network::Initialize();
				LOG_TRACE("Networking system initialized");
			}
			else
			{
				LOG_WARN("No network system loaded.");
			}

			// #TODO load scene later, like in Run() as it's more than just initializing
			Scenes::Initialize(); // #TODO Investigate other system dependencies as there are likely several

			Physics::Initialize();

			return eEngineMessage::_QSuccess;
		}

		eEngineMessage Framework::TearDown()
        {
            EventManager::Shutdown();
			Libs_TearDown();
			return eEngineMessage::_QSuccess;
		}

		void Framework::Run()
        {
            assert(m_Window != nullptr); // Don't forget to call Framework::Startup() to initialize the framework

			// #TODO check if(initialized) in case user defined simple API.
			// Might want to create another function for the game loop and
			// leave Run() smaller and abstracted from the functionality.
			m_IsRunning = true;

			/* Application Loop */
			int FPS_MAX = 120; // Maximum number of frames that can be made per second
			float FPS_MAX_DELTA = 1.0f / FPS_MAX;

			Time::NewFrame();

			while (m_Window->IsClosing() == false)
			{
                /* Game Loop */

				float deltaTime = Time::Delta();

				// if (deltaTime >= FPS_MAX_DELTA)
				{
					Time::NewFrame();

					Framework::NewFrame();

					Framework::PollInput();

					Framework::Update(deltaTime);

					Framework::Draw();

					// FPS
					//framesSincePrint++; // Frame rate tracking
					//timeSinceLastFrame = 0.0; // FPS_Max
				}
				// #TODO Fix delta time issues
				// else
				// {
				// 	Yield();
				// }
			}
		}

		void Framework::Stop()
		{
			m_IsRunning = false;
			m_Window->SetClosing(true);
		}

		void Framework::NewFrame()
		{
			Input::NewFrame();
			EventManager::ProcessEvents();
			Renderer::NewFrame();

			// NOTE: ImGUI::NewFrame() is in PollInput()!
		}

		void Framework::PollInput()
		{
			// #TODO Abstract libraries
            glfwPollEvents(); // #TODO Better GLFW interface?
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame(); // after Input gets reset, and after glfw input polling is done
            ImGui::NewFrame();
			// #TODO Tell input manager it is a new frame and it should update key states
		}

		void Framework::Update(float deltatime)
		{
			const ConfigData config = ConfigHelper::GetConfigData();
			if (config.systems.PhysicsEnabled)
			{
				Physics::Tick();
			}

			Scenes::Update(deltatime);

			if (Input::FrameKeyAction(eKeys::eKeys_Escape, eKeyState::eKeyState_Press))
			{
				Stop();
			}
		}

		void Framework::Draw()
		{
			Scenes::DrawCurrentScene();

			// #TODO Abstract libraries
			ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            ImGuiIO io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }

			m_Window->SwapBuffers();
		}

		bool Framework::StillRunning()
		{
			return m_IsRunning;
		}
	}
}
