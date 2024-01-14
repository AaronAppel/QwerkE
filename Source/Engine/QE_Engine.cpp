#include "QE_Engine.h"

#include "QF_Framework.h" // #TODO Review include order

#include "Libraries/FlatheadGames/MathHelpers.h"
#include "Libraries/glew/GL/glew.h"
#include "Libraries/glfw/GLFW/glfw3.h"

#define IMGUI_EDITOR

#ifdef IMGUI_EDITOR
#pragma warning( disable : 4244 )
#include "Libraries/imgui/imgui.h"
#include "Libraries/imgui/imgui_impl_glfw.h"
#include "Libraries/imgui/imgui_impl_opengl3.h"
#pragma warning( default : 4244 )

#include "QE_imgui_Editor.h"
#else
#pragma warning "Define editor library!"
#endif

#include "QC_ProgramArgs.h"

#include "QF_Profile.h"
#include "QF_Debugger.h"
#include "QF_Graphics_Header.h"
#include "QF_FrameBufferObject.h"
#include "QF_MeshFactory.h"
#include "QF_Renderer.h"
#include "QF_Input.h"
#include "QF_Resources.h"
#include "QF_EventManager.h"
#include "QF_Scene.h"
#include "QF_CallbackFunctions.h"
#include "QF_Physics.h"
#include "QF_Audio.h"
#include "QF_ShaderFactory.h"
#include "QF_Jobs.h"
#include "QF_Network.h"
#include "QF_glfw_Window.h"
#include "QF_Time.h"
#include "QF_Log.h"
#include "QF_ConfigHelper.h"
#include "QF_Scenes.h"
#include "QF_Window.h"
#include "QF_Windows.h"

namespace QwerkE {

	namespace Engine
    {
        static bool m_IsRunning = false;
        static Editor* m_Editor = nullptr;

		QwerkE::eEngineMessage Engine::Run(std::map<const char*, const char*> &args)
        {
			// #TODO Open or close console output window using config.systems.ConsoleOutputWindowEnabled

            Instrumentor::Get().BeginSession("Instrumentor", "instrumentor_log.json");
			PROFILE_SCOPE("Run"); // #TODO Review. Shouldn't need to persist constantly. Measure startup time instead.

			if (args.find(key_ProjectName) != args.end())
			{
				// Load project (reusable method)
				// Could find and save preferences file path for recent project(s)
				// Show options to choose from with a list of recent projects, ordered by date.
				// Add an option to auto-load most recently opened project
			}

			// #TODO check if(initialized) in case user defined simple API.
			// Might want to create another function for the game loop and
			// leave Run() smaller and abstracted from the functionality.

			if (Framework::Startup(ConfigsFolderPath("preferences.qpref")) == eEngineMessage::_QFailure)
            {
                LOG_CRITICAL("Qwerk Framework failed to load! Shutting down engine..."); // #TODO Shutdown properly
				Instrumentor::Get().EndSession();
				return eEngineMessage::_QFailure;
			}

			Scenes::GetCurrentScene()->SetIsEnabled(true);

			// #TODO Move to editor class
#ifdef dearimgui
			m_Editor = (Editor*)new imgui_Editor();
#else
			m_Editor = (Editor*)new ????_Editor();
#endif

			const EngineSettings engineSettings = ConfigHelper::GetConfigData().engineSettings;
			const unsigned int FPS_MAX = (int)(engineSettings.LimitFramerate) * engineSettings.MaxFramesPerSecond;
			const float FPS_MAX_DELTA = FPS_MAX ? 1.0f / FPS_MAX : 1.f / 120.f;

			double lastFrameTime = Time::Now();
			double timeUntilNextFrame = 0.0;

			Time::InitStartTime();
			m_IsRunning = true;
			while (m_IsRunning)
			{
				if (timeUntilNextFrame >= FPS_MAX_DELTA)
				{
					Engine::NewFrame();

					Engine::PollInput();

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

			Framework::TearDown();
			Instrumentor::Get().EndSession();

			return QwerkE::eEngineMessage::_QSuccess;
		}

		void Engine::Stop()
		{
			Framework::Stop();
			m_IsRunning = false;
		}

		void Engine::NewFrame()
		{
			Framework::NewFrame();
            m_Editor->NewFrame();
		}

		void Engine::PollInput()
        {
            PROFILE_SCOPE("Engine Input");
			Framework::PollInput();
		}

		void Engine::Update(float deltaTime)
        {
            PROFILE_SCOPE("Engine Update");

			Framework::Update(deltaTime);

			m_Editor->Update();

			if (Input::FrameKeyAction(eKeys::eKeys_Escape, eKeyState::eKeyState_Press))
			{
				Stop();
			}

			if (Input::FrameKeyAction(eKeys::eKeys_F, eKeyState::eKeyState_Press))
			{
				m_Editor->ToggleEditorUi();
			}
		}

		void Engine::Draw()
        {
			PROFILE_SCOPE("Engine Render");

			m_Editor->Draw();

			Framework::Draw();
		}
	}
}
