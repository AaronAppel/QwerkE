#include "Engine.h"
#include "Systems/ResourceManager.h"
#include "Systems/ServiceLocator.h"
#include "Engine_Enums.h"
#include "Utilities/Helpers.h"
#include "Libraries_Initialize.h"
#include "Events/EventManager.h"
#include "Scene/SceneManager.h"
#include "Factory/Factory.h"
#include "Editor/Editor.h"

// TODO: No Globals!
extern int g_WindowWidth = 1280, g_WindowHeight = 720; // (1280x720)(1600x900)(1920x1080)(2560x1440)
extern const char* g_WindowTitle = "QwerkE";

extern MyGLFW* g_MainWindow = nullptr;
extern GameCore* g_GameCore = nullptr;
extern const int g_NumPlayers; // Defined in InputManager.h
extern InputManager* g_InputManager = nullptr;
extern Controller* g_Player1Controller = nullptr;
extern XinputHandler* g_XinputHandler = nullptr;
extern bool g_Debugging = false;

extern float g_FrameRate = 0.0f;
extern double g_TimeSinceLastFrame = 0.0;

Engine::Engine()
{
	// init? maybe if _QTest defined or something
}

Engine::~Engine()
{
	// deletion checks
}

eEngineMessage Engine::Startup()
{
	if (Libs_Setup() == false) // setup libraries
	{
		ConsolePrint("\Startup(): Error loading libraries! Closing application\n");
		return eEngineMessage::_QFail; // failure
	}

    // TODO: Try to reduce or avoid order dependency in system creation

	// load and register systems
	// Audio, Networking, Graphics (Renderer, GUI), Utilities (Conversion, FileIO, Printing),
	// Physics, Event, Debug, Memory, Window, Application, Input, Resources
	ResourceManager* resourceManager = new ResourceManager();
	QwerkE::ServiceLocator::RegisterService(eEngineServices::Resource_Manager, resourceManager);

	InputManager* inputManager = new InputManager();
	QwerkE::ServiceLocator::RegisterService(eEngineServices::Input_Manager, inputManager);

	m_Window = glfwCreateWindow(g_WindowWidth, g_WindowHeight, g_WindowTitle, NULL, NULL);
    glfwSwapInterval(0); // TODO: Add VSynch control and toggling
	glfwMakeContextCurrent(m_Window);
    ImGui_ImplGlfwGL3_Init(m_Window, true);
	QwerkE::ServiceLocator::RegisterService(eEngineServices::AppWindow, m_Window);

    EventManager* eventManager = new EventManager();
    QwerkE::ServiceLocator::RegisterService(eEngineServices::Event_System, eventManager);

    SceneManager* sceneManager = new SceneManager();
    sceneManager->Initialize();
    QwerkE::ServiceLocator::RegisterService(eEngineServices::Scene_Manager, sceneManager);

    Factory* factory = new Factory();
    QwerkE::ServiceLocator::RegisterService(eEngineServices::Factory_Entity, factory);

    m_Editor = new Editor();
    QwerkE::ServiceLocator::RegisterService(eEngineServices::Editor, m_Editor);

	return QwerkE::ServiceLocator::ServicesLoaded();
}

eEngineMessage Engine::TearDown()
{
	delete (ResourceManager*)QwerkE::ServiceLocator::UnregisterService(eEngineServices::Resource_Manager);

	delete (InputManager*)QwerkE::ServiceLocator::UnregisterService(eEngineServices::Input_Manager);

	glfwDestroyWindow((GLFWwindow*)QwerkE::ServiceLocator::UnregisterService(eEngineServices::AppWindow));

    delete (EventManager*)QwerkE::ServiceLocator::UnregisterService(eEngineServices::Event_System);

    delete (SceneManager*)QwerkE::ServiceLocator::UnregisterService(eEngineServices::Scene_Manager);

    delete (Factory*)QwerkE::ServiceLocator::UnregisterService(eEngineServices::Factory_Entity);

	Libs_TearDown(); // unload libraries

	// TODO: Safety checks?
	return eEngineMessage::_QSuccess;
}

void Engine::Run()
{
	m_IsRunning = true;

	// Start FPS //
	// Deltatime
	double deltaTime = 0.0f; // Time between current frame and last frame
	double lastFrame = helpers_Time(); // Time of last frame initialized to current time
	// Limit framerate
	int FPS_MAX = 120; // maximum number of frames that can be run be second
	float FPS_MAX_DELTA = 1.0f / FPS_MAX;
    g_TimeSinceLastFrame = FPS_MAX; // Amount of seconds since the last frame ran initialized to run 1st time
	// Printing framerate
	float printPeriod = 3.0f; // Print period in seconds
	float timeSincePrint = printPeriod; // Seconds since last print initiliazed to print 1st frame
	short framesSincePrint = 0;
	// End FPS //

	while (glfwWindowShouldClose(m_Window) == false) // TODO: 'ESC' key down or something
	{
		// setup frame
		// Calculate deltatime of current frame
		double currentFrame = helpers_Time();
		deltaTime = currentFrame - lastFrame; // time since last frame
		lastFrame = currentFrame; // save last frame

        // FPS display + tracking
		if (timeSincePrint >= printPeriod) // print period
		{
			g_FrameRate = 1.0f / timeSincePrint * framesSincePrint;
			OutputPrint("\nFPS: %f", g_FrameRate); // FPS printout
            OutputPrint("\nFrames: %i", framesSincePrint); // Frames printout
			timeSincePrint = 0.0f;
			framesSincePrint = 0;
		}

		timeSincePrint += (float)deltaTime;
        g_TimeSinceLastFrame += deltaTime;

		if (g_TimeSinceLastFrame >= FPS_MAX_DELTA) // Run frame?
		{
			/* Game Loop */
			/* New Frame */
            glfwPollEvents(); // TODO: Better GLFW interface?
            ImGui_ImplGlfwGL3_NewFrame();

			/* Input */
			// g_MainWindow->CheckInput();
			// ProcessXInput();
			/* Logic */
			// Engine::Update();
			/* Render */
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Engine::Draw();

            ImGui::ShowDemoWindow();

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(m_Window);

            // FPS
			framesSincePrint++; // Framerate tracking
			g_TimeSinceLastFrame = 0.0; // FPS_Max
		}
		else
		{
			// skip frame
		}
	}
}

void Engine::NewFrame()
{
	// TODO: Reset things...
}

void Engine::Update()
{
	// TODO: Update editor and/or game code
	static int counter = 0;
	counter++;
	if (counter > 600)
	{
		glfwSetWindowShouldClose(m_Window, true);
	}
    ((EventManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Event_System))->QueueEvent();
}

void Engine::Draw()
{
	// TODO: Render game and editor
	// g_MainWindow->SwapBuffers(); // Flip new image (buffer) to screen

}