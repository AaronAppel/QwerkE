#include "QF_Framework.h"

#include "QF_Events.h"
#include "QF_Input.h"
#include "QF_Jobs.h"
#include "QF_Log.h"
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

			const EngineSettings& engineSettings = Settings::GetEngineSettings();

			Window::Initialize();

			Input::Initialize();
			// keyFuncId = Input::RegisterOnKeyEvent(eKeys_D, OnKeyFunc);
			// keyFuncId2 = Input::RegisterOnKeyEvent(eKeys_D, OnKeyFunc2);

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