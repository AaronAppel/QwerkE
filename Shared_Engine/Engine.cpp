#include "Engine.h"
#include "Systems/ResourceManager.h"
#include "Systems/ServiceLocator.h"
#include "Engine_Enums.h"
#include "Utilities/Helpers.h"

extern float g_FrameRate;
extern double g_TimeSinceLastFrame;

Engine::Engine()
{
}

Engine::~Engine()
{
}

eEngineMessage Engine::Startup()
{
	// load and register systems
	// Audio, Networking, Graphics (Renderer, GUI), Utilities (Conversion, FileIO, Printing),
	// Physics, Event, Debug, Memory, Window, Application, Input, Resources
	ResourceManager* resourceManager = new ResourceManager();
	QwerkE::ServiceLocator::RegisterService(eEngineServices::Resource_Manager, resourceManager);

	InputManager* inputManager = new InputManager();
	QwerkE::ServiceLocator::RegisterService(eEngineServices::Input_Manager, inputManager);

	return QwerkE::ServiceLocator::ServicesLoaded();
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

	while (m_IsRunning) // TODO: 'ESC' key down or something
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
		// MAX_FPS (limit framerate)
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
	if (counter > 1200)
	{
		m_IsRunning = false;
	}
}

void Engine::Draw()
{
	// TODO: Render game and editor
	// g_MainWindow->SwapBuffers(); // Flip new image (buffer) to screen

}