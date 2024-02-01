#include "QF_Framework.h"

#include "Libraries/imgui/QC_imgui.h"

#include "QF_Audio.h"
#include "QF_CallbackFunctions.h"
#include "QF_ConfigHelper.h"
#include "QF_DataManager.h"
#include "QF_Debug.h"
#include "QF_Debugger.h"
#include "QF_Enums.h"
#include "QF_EventManager.h"
#include "QF_Factory.h"
#include "QF_FileSystem.h"
#include "QF_Graphics_Header.h"
#include "QF_Input.h"
#include "QF_Jobs.h"
#include "QF_Log.h"
#include "QF_MeshFactory.h"
#include "QF_Network.h"
#include "QF_Physics.h"
#include "QF_Renderer.h"
#include "QF_Resources.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_ShaderFactory.h"
#include "QF_Time.h"
#include "QF_Window.h"

namespace QwerkE
{

	namespace Framework
	{

		eOperationResult Framework::Startup()
		{
			return Startup(ConfigsFolderPath(null_config));
		}

		eOperationResult Framework::Startup(const std::string configFilePath)
		{
			// #TODO Load libraries dynamically. Need functions to load .dlls
			// #TODO Try to reduce or avoid order dependency in system creation

            Log::Initialize();

			const ConfigData& configData = ConfigHelper::GetConfigData();
			const UserData& userData = ConfigHelper::GetUserData();
			const ProjectData& projectData = ConfigHelper::GetProjectData();

			ConfigHelper::LoadConfigData(configFilePath);
			ConfigHelper::LoadUserData(ConfigsFolderPath(null_preferences));
			ConfigHelper::LoadProjectData(ProjectsFolderPath(null_project));

			ConfigHelper::SaveConfigData(); // #TODO Remove when testing finished
			ConfigHelper::SaveUserData();
			ConfigHelper::SaveProjectData();

			Window::Initialize();

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
			Window::SwapBuffers();

			EventManager::Initialize();

			if (configData.systems.PhysicsEnabled)
			{
				Physics::Initialize();
			}
			else
			{
				LOG_WARN("No physics system loaded.");
			}

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

			return eOperationResult::Success;
		}

		eOperationResult Framework::TearDown()
        {
            EventManager::Shutdown();
			Window::Shutdown();
			Log::Shutdown();
			return eOperationResult::Success;
		}

		void Framework::Run()
        {
			int FPS_MAX = 120;
			float FPS_MAX_DELTA = 1.0f / FPS_MAX;

			Time::InitStartTime();

			while (Window::CloseRequested() == false)
			{
				float deltaTime = Time::FrameDelta();

				// if (deltaTime >= FPS_MAX_DELTA)
				{
					Framework::NewFrame();

					Framework::Update(deltaTime);

					Framework::Draw();

					Time::EndFrame();
					// FPS
					//framesSincePrint++; // Frame rate tracking
					//timeSinceLastFrame = 0.0; // FPS_Max
				}
			}
		}

		void Framework::Stop()
		{
			Window::RequestClose();
		}

		void Framework::NewFrame()
		{
			Input::NewFrame();
			EventManager::ProcessEvents(); // #TODO Review ordering
			Renderer::NewFrame();
			Window::NewFrame();
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
			Window::Render();
		}

		bool Framework::StillRunning()
		{
			return Window::CloseRequested() == false;
		}

	}

}
