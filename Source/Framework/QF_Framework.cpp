#include "QF_Framework.h"

#include "Libraries/imgui/QC_imgui.h"

#include "QF_Enums.h"
#include "QF_Audio.h"
#include "QF_ConfigHelper.h"
#include "QF_DataManager.h"
#include "QF_Debug.h"
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

    static Window* s_Window = nullptr;
    static bool s_IsRunning = false;

	namespace Framework
	{
		eEngineMessage Framework::Startup()
		{
			return Startup(ConfigsFolderPath(null_config));
		}

		eEngineMessage Framework::Startup(const std::string configFilePath)
		{
			// #TODO Load libraries dynamically. Need functions to load .dlls
			// #TODO Try to reduce or avoid order dependency in system creation

            Log::Initialize();

			ConfigHelper::LoadConfigData(configFilePath);
			ConfigHelper::LoadUserData(ConfigsFolderPath(null_preferences));

			const ConfigData& configData = ConfigHelper::GetConfigData();
			const UserData& userData = ConfigHelper::GetUserData();

			Windows::Initialize();
			s_Window = Windows::GetWindow(0);

            Input::Initialize();

            if (configData.systems.AudioEnabled && Audio::Initialize())
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
			s_Window->SwapBuffers();

			EventManager::Initialize();

			if (configData.systems.NetworkingEnabled)
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

			if (configData.systems.PhysicsEnabled)
			{
				Physics::Initialize();
			}
			else
			{
				LOG_WARN("No physics system loaded.");
			}

			return eEngineMessage::_QSuccess;
		}

		eEngineMessage Framework::TearDown()
        {
            EventManager::Shutdown();
			Windows::Shutdown();
			Log::Shutdown();
			return eEngineMessage::_QSuccess;
		}

		void Framework::Run()
        {
            ASSERT(s_Window != nullptr, "Window not created! Check that Startup was called and returned successfully");

			// #TODO check if(initialized) in case user defined simple API.
			// Might want to create another function for the game loop and
			// leave Run() smaller and abstracted from the functionality.
			s_IsRunning = true;

			/* Application Loop */
			int FPS_MAX = 120; // Maximum number of frames that can be made per second
			float FPS_MAX_DELTA = 1.0f / FPS_MAX;

			Time::InitStartTime();

			while (s_Window->IsClosing() == false)
			{
                /* Game Loop */

				float deltaTime = Time::FrameDelta();

				// if (deltaTime >= FPS_MAX_DELTA)
				{
					Framework::NewFrame();

					Framework::PollInput();

					Framework::Update(deltaTime);

					Framework::Draw();

					Time::EndFrame();
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
			s_IsRunning = false;
			s_Window->SetClosing(true);
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
			Physics::Tick();

			Scenes::Update(deltatime);

			if (Input::FrameKeyAction(eKeys::eKeys_Escape, eKeyState::eKeyState_Press))
			{
				Stop();
			}
		}

		void Framework::Draw()
		{
			Scenes::DrawCurrentScene();
			Windows::Render();
		}

		bool Framework::StillRunning()
		{
			return s_IsRunning;
		}
	}
}
