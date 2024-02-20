#include "QF_Framework.h"

#include "Libraries/imgui/QC_imgui.h"

#include "QF_Audio.h"
#include "QF_CallbackFunctions.h"
#include "QF_Debug.h"
#include "QF_Debugger.h"
#include "QF_Enums.h"
#include "QF_EventManager.h"
#include "QF_FileSystem.h"
#include "QF_Graphics_Header.h"
#include "QF_Input.h"
#include "QF_Jobs.h"
#include "QF_Log.h"
#include "QF_Network.h"
#include "QF_Physics.h"
#include "QF_Renderer.h"
#include "QF_Resources.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Settings.h"
#include "QF_ShaderFactory.h"
#include "QF_Time.h"
#include "QF_Window.h"

#include "QF_Serialization.h" // #TODO Remove after testing

namespace QwerkE {

	namespace Framework {

		eOperationResult Framework::Startup()
		{
			return Startup(SettingsFolderPath(null_config));
		}

		eOperationResult Framework::Startup(const std::string engineSettingsPath)
		{
			// #TODO Load libraries dynamically. Need functions to load .dlls
			// #TODO Try to reduce or avoid order dependency in system creation

			// const char* newName = new char[5] { 'N','a','m', 'e', '\0' };
			// unsigned  long long counter = UINT64_MAX - 1;
			// const unsigned long long max = UINT64_MAX - 1;
			// // const char* newName = new char[12] { 'N', '2', '1', '4', '7', '4', '8', '3', '6', '4', '7', '\0' };
			// const char* newName = new char[22] { 'N', '1', '8', '4', '4', '6', '7', '4', '4', '0', '7', '3', '7', '0', '9',
			// 	'5', '5', '1', '6', '1', '4', '\0' };
			//
			// while (true)
			// {
			// 	const char* oldName = newName;
			// 	newName = NumberAppendOrIncrement(newName);
			// 	delete[] oldName;
			//
			// 	Log::Console(newName);
			// 	Log::Console("\n");
			//
			// 	std::string value("N");
			// 	value.append(std::to_string(counter + 1));
			// 	if (value != newName)
			// 	{
			// 		int bp = 0;
			// 	}
			//
			// 	if (counter % 100 == 0)
			// 	{
			// 		int bp = 0;
			// 	}
			// 	++counter;
			// }
			// delete[] newName;

            Log::Initialize();

			//GameObject a;
			//Serialization::DeserializeObjectFromFile("A:/GitHub/QwerkE/Assets/Scenes/gameobject.qtest", a);

			Settings::LoadEngineSettings(engineSettingsPath);
			Settings::LoadProjectSettings(ProjectsFolderPath("Project1.qproj"));
			Settings::LoadUserSettings(SettingsFolderPath(null_preferences));

			const EngineSettings& engineSettings = Settings::GetEngineSettings();
			const ProjectSettings& projectSettings = Settings::GetProjectSettings();
			const UserSettings& userSettings = Settings::GetUserSettings();

			Window::Initialize();

            Input::Initialize();

            if (engineSettings.audioEnabled && Audio::Initialize())
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

			if (engineSettings.physicsEnabled)
			{
				Physics::Initialize();
			}
			else
			{
				LOG_WARN("No physics system loaded.");
			}

			if (engineSettings.networkingEnabled)
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
			Scenes::Shutdown();
            EventManager::Shutdown();
			Window::Shutdown();
			Log::Shutdown();
			return eOperationResult::Success;
		}

		void Framework::Run() // #TODO Review framework Run() loop
        {
			int FPS_MAX = 120;
			float FPS_MAX_DELTA = 1.0f / FPS_MAX;

			Time::InitStartTime();

			while (Window::CloseRequested() == false)
			{
				float deltaTime = Time::FrameDelta();

				Framework::NewFrame();

				Framework::Update(deltaTime);

				Framework::Draw();

				Time::EndFrame();
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
