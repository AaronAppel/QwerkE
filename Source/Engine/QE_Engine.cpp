#include "QE_Engine.h"

#include "QF_Framework.h"

#include "glew/GL/glew.h"
#include "glfw/GLFW/glfw3.h"

#define IMGUI_EDITOR

#ifdef IMGUI_EDITOR
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "QE_imgui_Editor.h"
#else
#pragma warning "Define editor library!"
#endif

#include "QC_Helpers.h"
#include "QC_ProgramArgs.h"

#include "QF_Profile.h"
#include "QF_Debugger.h"

#include "../QwerkE_Framework/Source/Core/Graphics/Graphics_Header.h"
#include "../QwerkE_Framework/Source/Core/Graphics/DataTypes/FrameBufferObject.h"
#include "../QwerkE_Framework/Source/Core/Graphics/Mesh/MeshFactory.h"
#include "../QwerkE_Framework/Source/Core/Graphics/Renderer.h"
#include "../QwerkE_Framework/Source/Core/Input/Input.h"
#include "QF_Resources.h"
#include "../QwerkE_Framework/Source/Core/Events/EventManager.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Scene.h"
#include "QF_CallbackFunctions.h"
#include "../QwerkE_Framework/Source/Core/Physics/Physics.h"
#include "QF_Audio.h"
#include "../QwerkE_Framework/Source/Core/Graphics/ShaderFactory/ShaderFactory.h"
#include "QF_Jobs.h"
#include "../QwerkE_Framework/Source/Core/Network/Network.h"
#include "QF_Window.h"
#include "QF_Windows.h"
#include "QF_glfw_Window.h"
#include "QF_Time.h"
#include "QF_Log.h"
#include "../QwerkE_Framework/Source/Core/DataManager/ConfigHelper.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Scenes.h"

namespace QwerkE {

	namespace Engine
    {
        static bool m_IsRunning = false;
        static Editor* m_Editor = nullptr;

		QwerkE::eEngineMessage Engine::Run(std::map<const char*, const char*> &args)
        {
			// #TODO Open or close console output window using config.systems.ConsoleOutputWindowEnabled

            Instrumentor::Get().BeginSession("Instrumentor", "instrumentor_log.json");
			PROFILE_SCOPE("Run");

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
                Log::Safe("Qwerk Framework failed to load. Shutting down engine.");
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
			const float FPS_MAX_DELTA = FPS_MAX ? 1.0f / FPS_MAX : 0;

			float lastIterationTime = Time::Now();
			float timeUntilNextFrame = 0.f;

			m_IsRunning = true;
			while (m_IsRunning)
			{
				timeUntilNextFrame -= Time::Now() - lastIterationTime;
				lastIterationTime = Time::Now();

				if (timeUntilNextFrame <= 0.f)
				{
					Time::NewFrame();

					Engine::NewFrame();

					Engine::PollInput();

					Engine::Update(Time::Delta());

                    Engine::Draw();

					timeUntilNextFrame = FPS_MAX_DELTA;
				}
                else
                {
                	YieldProcessor();
                }
			}

            Instrumentor::Get().EndSession();
			Framework::TearDown();

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
