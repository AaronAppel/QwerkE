#include "QE_Engine.h"

#include "QF_Framework.h" // #TODO Review include order

#include "Libraries/FlatheadGames/MathHelpers.h"
#include "Libraries/glew/GL/glew.h"
#include "Libraries/glfw/GLFW/glfw3.h"

#define IMGUI_EDITOR

#ifdef IMGUI_EDITOR
#include "Libraries/imgui/QC_imgui.h"
#else
#pragma warning "Define editor library!"
#endif

#include "QF_Profile.h"
#include "QF_Debugger.h"
#include "QF_FrameBufferObject.h"
#include "QF_Renderer.h"
#include "QF_Input.h"
#include "QF_Assets.h"
#include "QF_Scene.h"
#include "QF_Physics.h"
#include "QF_Audio.h"
#include "QF_ShaderFactory.h"
#include "QF_Jobs.h"
#include "QF_Network.h"
#include "QF_Time.h"
#include "QF_Log.h"
#include "QF_Scenes.h"
#include "QF_Settings.h"
#include "QF_Window.h"

#include "QE_Defines.h"
#include "QE_Editor.h"
#include "QE_ProgramArgs.h"

namespace QwerkE {

	namespace Engine {

		QwerkE::eOperationResult Engine::Run(const std::map<const char*, const char*>& programArgPairs)
        {
			Log::Print("-- Qwerk Engine %f %s", QWERKE_VERSION, "--\n");

            Instrumentor::Get().BeginSession("Instrumentor", "instrumentor_log.json");

			{	PROFILE_SCOPE("Run Startup");

				if (Framework::Startup(SettingsFolderPath(null_config)) == eOperationResult::Failure)
				{
					LOG_CRITICAL("Error while loading! Shutting down QwerkE...");
					Instrumentor::Get().EndSession();
					return eOperationResult::Failure;
				}

				if (programArgPairs.find(key_ProjectFileName) != programArgPairs.end())
				{
					const char* projectName = programArgPairs.find(key_ProjectFileName)->second;
					const char* projectFileName = StringAppend(projectName, ".", projects_file_ext);
					projectFileName = StringAppend(projectName, "/", projectFileName);
					// Settings::LoadProjectData(ProjectsFolderPath(projectFileName));

					// #TODO Load project folder
					// Could find and save preferences file path for recent project(s)
					// Show options to choose from with a list of recent projects, ordered by date.
					// Add an option to auto-load most recently opened project
				}

				if (programArgPairs.find(key_UserName) != programArgPairs.end())
				{
					const std::string userName = programArgPairs.find(key_UserName)->second;
					std::string userConfigFilePath = userName + ".";
					userConfigFilePath += preferences_ext;
					// Settings::LoadUserSettings(SettingsFolderPath(userConfigFilePath.c_str())); // #TODO Framework also loads user data
				}

				Editor::Initialize();
			}

			const EngineSettings& engineSettings = Settings::GetEngineSettings();
			const unsigned int FPS_MAX = (int)(engineSettings.limitFramerate) * engineSettings.maxFramesPerSecond;
			const float FPS_MAX_DELTA = FPS_MAX ? 1.0f / FPS_MAX : 1.f / 120.f;

			double lastFrameTime = Time::Now();
			double timeUntilNextFrame = 0.0;

			Time::InitStartTime();

			while (Window::CloseRequested() == false)
			{
				if (timeUntilNextFrame >= FPS_MAX_DELTA)
				{
					Engine::NewFrame();

					Engine::Update(Time::FrameDelta());

					Engine::Draw();

					Time::EndFrame();

					timeUntilNextFrame = 0.0;
				}
				else
				{
					YieldProcessor();
				}

				const double now = Time::Now();
				const double frameDelta = now - lastFrameTime;
				timeUntilNextFrame += frameDelta;
				lastFrameTime = now;
			}

			Settings::SaveEngineSettings();

			Framework::TearDown();
			Instrumentor::Get().EndSession();

			return QwerkE::eOperationResult::Success;
		}

		void Engine::Stop()
		{
			Framework::Stop();
		}

		void Engine::NewFrame()
		{
			PROFILE_SCOPE("Engine NewFrame");

			Framework::NewFrame();
		}

		void Engine::Update(float deltaTime)
        {
            PROFILE_SCOPE("Engine Update");

			Framework::Update(deltaTime);

			if (Input::FrameKeyAction(eKeys::eKeys_Escape, eKeyState::eKeyState_Press))
			{
				Stop();
			}
		}

		void Engine::Draw()
        {
			PROFILE_SCOPE("Engine Render");

			Editor::Draw();

			Framework::Draw();
		}

	}
}
