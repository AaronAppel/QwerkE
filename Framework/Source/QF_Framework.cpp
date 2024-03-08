#include "QF_Framework.h"

#include "QF_Audio.h"
#include "QF_Assets.h"
#include "QF_Events.h"
#include "QF_Input.h"
#include "QF_Jobs.h"
#include "QF_Log.h"
#include "QF_Renderer.h"
#include "QF_Scenes.h"
#include "QF_Settings.h"
#include "QF_Window.h"

namespace QwerkE {

	namespace Framework {

		eOperationResult Initialize()
		{
			Log::Initialize();

			Events::Initialize();

			Settings::LoadEngineSettings(""); // engineSettingsPath
			Settings::LoadProjectSettings(ProjectsFolderPath("Project1.qproj"));
			Settings::LoadUserSettings(SettingsFolderPath(null_preferences));

			Window::Initialize();

			Input::Initialize();
			// keyFuncId = Input::RegisterOnKeyEvent(eKeys_D, OnKeyFunc);
			// keyFuncId2 = Input::RegisterOnKeyEvent(eKeys_D, OnKeyFunc2);

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

			return eOperationResult::Success;
		}

		eOperationResult Shutdown()
		{
			Scenes::Shutdown();
			Events::Shutdown();
			Window::Shutdown();
			Log::Shutdown();
			return eOperationResult::Success;
		}

		void NewFrame()
		{
			Input::NewFrame();
			Events::ProcessEvents();
			Jobs::ProcessTasks();
			Renderer::NewFrame();
			Window::NewFrame();
		}

		void Update(float deltatime)
		{
			Scenes::Update(deltatime);
		}

		void Draw()
		{
			Scenes::DrawCurrentScene();
			Window::Render();
		}

	}

}