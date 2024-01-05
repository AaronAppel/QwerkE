#include "Framework.h"

#include "Source/Headers/QwerkE_Defines.h"

#ifdef GLFW3 // #TODO Move library dependent logic into appropriate class, like Window
#include "glew/GL/glew.h"
#include "glfw/GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#endif

#include "../QwerkE_Framework/Source/Headers/QwerkE_Enums.h"
#include "../QwerkE_Framework/Source/Headers/Libraries_Initialize.h"

#include "QC_Helpers.h"

#include "../QwerkE_Framework/Source/Core/Audio/Audio.h"
#include "../QwerkE_Framework/Source/Core/DataManager/ConfigHelper.h"
#include "../QwerkE_Framework/Source/Core/DataManager/DataManager.h"
#include "../QwerkE_Framework/Source/Core/Events/EventManager.h"
#include "../QwerkE_Framework/Source/Core/Factory/Factory.h"
#include "../QwerkE_Framework/Source/Core/Graphics/Mesh/MeshFactory.h"
#include "../QwerkE_Framework/Source/Core/Graphics/Graphics_Header.h"
#include "../QwerkE_Framework/Source/Core/Graphics/Renderer.h"
#include "../QwerkE_Framework/Source/Core/Graphics/ShaderFactory/ShaderFactory.h"
#include "../QwerkE_Framework/Source/Core/Input/Input.h"
#include "../QwerkE_Framework/Source/Core/Jobs/Jobs.h"
#include "../QwerkE_Framework/Source/Core/Network/Network.h"
#include "../QwerkE_Framework/Source/Core/Physics/Physics.h"
#include "../QwerkE_Framework/Source/Core/Resources/Resources.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Scene.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Scenes.h"
#include "../QwerkE_Framework/Source/Core/Time/Time.h"
#include "../QwerkE_Framework/Source/Core/Window/Window.h"
#include "../QwerkE_Framework/Source/Core/Window/Windows.h"
#include "../QwerkE_Framework/Source/Core/Window/glfw_Window.h"
#include "../QwerkE_Framework/Source/Core/Window/CallbackFunctions.h"
#include "../QwerkE_Framework/Source/Debug/Debugger/Debugger.h"
#include "../QwerkE_Framework/Source/Debug/Log/Log.h"
#include "../QwerkE_Framework/Source/FileSystem/FileSystem.h"

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
            assert(m_Window != nullptr); // Don't forget to call Framework::Startup() to initiailize the framework

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

				double deltaTime = Time::Delta();

				// if (deltaTime >= FPS_MAX_DELTA)
				{
					Time::NewFrame();

					/* New Frame */
					Framework::NewFrame();

					/* Input */
					Framework::PollInput();

					/* Logic */
					Framework::Update(deltaTime);

					/* Render */
					Framework::Draw();

					// FPS
					//framesSincePrint++; // Framerate tracking
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

		void Framework::Update(double deltatime)
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
