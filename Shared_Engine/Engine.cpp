#include "Engine.h"
#include "Systems/ResourceManager.h"
#include "Systems/ServiceLocator.h"
#include "Engine_Enums.h"
#include "Utilities/Helpers.h"
#include "Libraries_Initialize.h"

extern int g_WindowWidth = 1280, g_WindowHeight = 720; // (1280x720)(1600x900)(1920x1080)(2560x1440)
extern const char* g_WindowTitle = "QwerkE";

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

	// load and register systems
	// Audio, Networking, Graphics (Renderer, GUI), Utilities (Conversion, FileIO, Printing),
	// Physics, Event, Debug, Memory, Window, Application, Input, Resources
	ResourceManager* resourceManager = new ResourceManager();
	QwerkE::ServiceLocator::RegisterService(eEngineServices::Resource_Manager, resourceManager);

	InputManager* inputManager = new InputManager();
	QwerkE::ServiceLocator::RegisterService(eEngineServices::Input_Manager, inputManager);

	m_Window = glfwCreateWindow(g_WindowWidth, g_WindowHeight, g_WindowTitle, NULL, NULL);
	glfwMakeContextCurrent(m_Window);
	QwerkE::ServiceLocator::RegisterService(eEngineServices::AppWindow, m_Window);

	return QwerkE::ServiceLocator::ServicesLoaded();
}

eEngineMessage Engine::TearDown()
{
	delete (ResourceManager*)QwerkE::ServiceLocator::UnregisterService(eEngineServices::Resource_Manager);

	delete (InputManager*)QwerkE::ServiceLocator::UnregisterService(eEngineServices::Input_Manager);

	glfwDestroyWindow((GLFWwindow*)QwerkE::ServiceLocator::UnregisterService(eEngineServices::AppWindow));

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
	double lastFrame = 0.0f; // Time of last frame
	g_TimeSinceLastFrame = 0.0f; // Amount of seconds since the last frame ran
	// Limit framerate
	int FPS_MAX = 120; // maximum number of frames that can be run be second
	float FPS_MAX_DELTA = 1.0f / FPS_MAX;
	// Printing framerate
	float printPeriod = 3.0f; // Print period in seconds
	float timeSincePrint = 0.0f;
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
		if (timeSincePrint > printPeriod) // print period
		{
			g_FrameRate = framesSincePrint / timeSincePrint;
			OutputPrint("\nFPS: %f", g_FrameRate); // FPS printout
			timeSincePrint = 0.0f;
			framesSincePrint = 0;
		}

		timeSincePrint += (float)deltaTime;
		g_TimeSinceLastFrame += deltaTime;

		if (g_TimeSinceLastFrame >= FPS_MAX_DELTA) // Run frame?
		{
			/* Game Loop */
			// new frame
			/* Input */
			// g_MainWindow->CheckInput();
			// ProcessXInput();
			/* Logic */
			Engine::Update();
			/* Render */
			Engine::Draw();

			framesSincePrint++; // Framerate tracking
			g_TimeSinceLastFrame = 0.0f; // FPS_Max
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
}

void Engine::Draw()
{
	// TODO: Render game and editor
	// g_MainWindow->SwapBuffers(); // Flip new image (buffer) to screen

}