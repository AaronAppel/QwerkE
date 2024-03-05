#include "QF_Framework.h"

#include "Libraries/imgui/QC_imgui.h"

#include "QF_Audio.h"
#include "QF_Debug.h"
#include "QF_Enums.h"
#include "QF_Events.h"
#include "QF_FileSystem.h"
#include "QF_Graphics_Header.h"
#include "QF_Input.h"
#include "QF_Jobs.h"
#include "QF_Log.h"
#include "QF_Network.h"
#include "QF_Renderer.h"
#include "QF_Assets.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Settings.h"
#include "QF_ShaderFactory.h" // #TODO Remove
#include "QF_Time.h"
#include "QF_Window.h"

#include "QF_Serialization.h" // #TODO Remove after testing

namespace QwerkE {

	namespace Framework {

		eOperationResult Framework::Startup()
		{
			return Startup(SettingsFolderPath(null_config));
		}

		static u16 keyFuncId = 0;
		void OnKeyFunc(eKeys key, eKeyState state)
		{
			LOG_TRACE("{0} Key {1} state {2}", __FUNCTION__, (char)key, (int)state);
			Input::UnregisterOnKeyEvent(eKeys_D, keyFuncId);
		}

		static u16 keyFuncId2 = 0;
		void OnKeyFunc2(eKeys key, eKeyState state)
		{
			LOG_TRACE("{0} Key {1} state {2}", __FUNCTION__, (char)key, (int)state);
			Input::UnregisterOnKeyEvent(eKeys_D, keyFuncId2);
		}

		eOperationResult Framework::Startup(const std::string engineSettingsPath)
		{
            Log::Initialize();

			Events::Initialize();

			Settings::LoadEngineSettings(engineSettingsPath);
			Settings::LoadProjectSettings(ProjectsFolderPath("Project1.qproj"));
			Settings::LoadUserSettings(SettingsFolderPath(null_preferences));

			Window::Initialize();

            Input::Initialize();
			keyFuncId = Input::RegisterOnKeyEvent(eKeys_D, OnKeyFunc);
			keyFuncId2 = Input::RegisterOnKeyEvent(eKeys_D, OnKeyFunc2);

			const EngineSettings& engineSettings = Settings::GetEngineSettings();
            if (engineSettings.audioEnabled && Audio::Initialize())
            {
                LOG_TRACE("Audio system initialized with OpenAL.");
            }
            else
            {
				LOG_WARN("No audio system loaded.");
            }

            Assets::Initialize();

			Renderer::Initialize();
			Renderer::DrawFont("Loading...", 300.f, 100.f, 5.f);
			Window::SwapBuffers();

			if (engineSettings.networkingEnabled)
			{
				Network::Initialize();
				LOG_TRACE("Networking system initialized");
			}
			else
			{
				LOG_WARN("No network system loaded.");
			}

			Scenes::Initialize();

			return eOperationResult::Success;
		}

		eOperationResult Framework::TearDown()
        {
			Scenes::Shutdown();
			Events::Shutdown();
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
			Events::ProcessEvents();
			Jobs::ProcessTasks();
			Renderer::NewFrame();
			Window::NewFrame();
		}

		void Framework::Update(float deltatime)
		{
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
