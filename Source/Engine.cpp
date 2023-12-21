#include "Engine.h"

#include "../QwerkE_Framework/Source/Framework.h"

#include "../QwerkE_Framework/Libraries/glew/GL/glew.h"
#include "../QwerkE_Framework/Libraries/glfw/GLFW/glfw3.h"

#define IMGUI_EDITOR

#ifdef IMGUI_EDITOR
#include "../QwerkE_Framework/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/Libraries/imgui/imgui_impl_glfw.h"
#include "../QwerkE_Framework/Libraries/imgui/imgui_impl_opengl3.h"

#include "Editor/imgui_Editor/imgui_Editor.h"
#else
#pragma warning "Define editor library!"
#endif

#include "../QwerkE_Framework/Source/Debug/Profiler/Profiler.h"
#include "../QwerkE_Framework/Source/Debug/Debugger/Debugger.h"

#include "../QwerkE_Framework/Source/Core/Graphics/Graphics_Header.h"
#include "../QwerkE_Framework/Source/Core/Graphics/DataTypes/FrameBufferObject.h"
#include "../QwerkE_Framework/Source/Core/Graphics/Mesh/MeshFactory.h"
#include "../QwerkE_Framework/Source/Core/Graphics/Renderer.h"
#include "../QwerkE_Framework/Source/Core/Input/Input.h"
#include "../QwerkE_Framework/Source/Core/Resources/Resources.h"
#include "../QwerkE_Framework/Source/Core/Events/EventManager.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Scene.h"
#include "../QwerkE_Framework/Source/Core/Window/CallbackFunctions.h"
#include "../QwerkE_Framework/Source/Core/Physics/Physics.h"
#include "../QwerkE_Framework/Source/Core/Audio/Audio.h"
#include "../QwerkE_Framework/Source/Core/Graphics/ShaderFactory/ShaderFactory.h"
#include "../QwerkE_Framework/Source/Core/Jobs/Jobs.h"
#include "../QwerkE_Framework/Source/Core/Network/Network.h"
#include "../QwerkE_Framework/Source/Core/Window/Window.h"
#include "../QwerkE_Framework/Source/Core/Window/Windows.h"
#include "../QwerkE_Framework/Source/Core/Window/glfw_Window.h"
#include "../QwerkE_Framework/Source/Core/Time/Time.h"

#include "../QwerkE_Framework/Source/Utilities/Helpers.h"
#include "../QwerkE_Framework/Source/Utilities/ProgramArgs.h"

namespace QwerkE {

	namespace Engine
    {
        // Private engine variables
        static bool m_IsRunning = false; // TODO: Remove extra variable
        static Editor* m_Editor = nullptr;

		void Engine::Run(std::map<const char*, const char*> &args)
        {
            Instrumentor::Get().BeginSession("Instrumentor", "instrumentor_log.json");
			PROFILE_SCOPE("Run");

			// Handle program arguments
			if (args.find(key_ProjectName) != args.end())
			{
				// Load project (reusable method)
				// Could find and save preferences file path for recent project(s)
				// Show options to choose from with a list of recent projects, ordered by date.
				// Add an option to auto-load most recently opened project
			}

			// TODO: check if(initialized) in case user defined simple API.
			// Might want to create another function for the game loop and
			// leave Run() smaller and abstracted from the functionality.

			std::uint_fast8_t flags = 0;

			// TEMP: Turn off components like this until the data can be read from a file
			flags &= ~Flag_Physics;
			flags &= ~Flag_Renderer;
			flags &= ~Flag_Audio;

			if (Framework::Startup(ConfigsFolderPath("preferences.qpref"), flags) == eEngineMessage::_QFailure)
            {
                Log::Safe("Qwerk Framework failed to load. Shutting down engine.");
				return;
			}

			Scenes::GetCurrentScene()->SetIsEnabled(true);

			m_IsRunning = true;

			// TODO: Resolve missing definition
			// Renderer::Initialize();

			// TODO: Move to editor class
#ifdef dearimgui
			m_Editor = (Editor*)new imgui_Editor();
#else
			m_Editor = (Editor*)new ????_Editor();
#endif // editor

			// TODO: Move this to a window class
			const unsigned char FPS_MAX = 10; // #TODO Not working
			const double FPS_MAX_DELTA = 1.0 / FPS_MAX;

            /* Application Loop */
			double deltaTime = 0.0;
			double elapsedTime = 0.0;

			while (m_IsRunning)
			{
                Time::NewFrame();

				deltaTime = Time::Delta();
				elapsedTime += deltaTime;
				// if (elapsedTime >= FPS_MAX_DELTA)
				{
					/* New Frame */
					Engine::NewFrame();

					/* Input */
					Engine::PollInput();

					/* Logic */
					Engine::Update(elapsedTime);

					/* Render */
                    Engine::Draw();

                    elapsedTime = 0.0;
				}
				// TODO : Fix yield that broke after deltatime changes
                // else
                // {
                // 	YieldProcessor(); // TODO: Look into proper yield behaviour
                // }
			}

            Instrumentor::Get().EndSession();
			Framework::TearDown();
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

		void Engine::Update(double deltaTime)
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
