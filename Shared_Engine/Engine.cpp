#include "Engine.h"
#include "Systems/ResourceManager.h"
#include "Systems/ServiceLocator.h"
#include "Engine_Enums.h"
#include "../../../Shared_Generic/Utilities/Helpers.h"
#include "../../../Shared_Generic/Libraries_Initialize.h"
#include "Events/EventManager.h"
#include "Scene/SceneManager.h"
#include "Factory/Factory.h"
#include "Editor/Editor.h"
#include "Systems/Graphics/Sprite/Sprite.h"
#include "Systems/Graphics_Header.h"
#include "Systems/Graphics/FBO/FrameBufferObject.h"
#include "CallbackFunctions.h"
#include "Systems/PhysicsManager.h"
#include "Systems/Renderer.h"
#include "Systems/MessageManager.h"

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

FrameBufferObject* g_FBO = new FrameBufferObject();

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

	m_SceneManager = new SceneManager();
    QwerkE::ServiceLocator::RegisterService(eEngineServices::Scene_Manager, m_SceneManager);

    Factory* factory = new Factory();
    QwerkE::ServiceLocator::RegisterService(eEngineServices::Factory_Entity, factory);

    m_Editor = new Editor();
    QwerkE::ServiceLocator::RegisterService(eEngineServices::Editor, m_Editor);

	PhysicsManager* physicsManager = new PhysicsManager();
	QwerkE::ServiceLocator::RegisterService(eEngineServices::PhysicsManager, physicsManager);

	MessageManager* messageManager = new MessageManager();
	QwerkE::ServiceLocator::RegisterService(eEngineServices::MessageManager, messageManager);

	Renderer* renderer = new Renderer();
	QwerkE::ServiceLocator::RegisterService(eEngineServices::Renderer, renderer);

	m_SceneManager->Initialize(); // Order Dependency

	return QwerkE::ServiceLocator::ServicesLoaded();
}

eEngineMessage Engine::TearDown()
{
	delete (ResourceManager*)QwerkE::ServiceLocator::UnregisterService(eEngineServices::Resource_Manager);

	delete (InputManager*)QwerkE::ServiceLocator::UnregisterService(eEngineServices::Input_Manager);

	glfwDestroyWindow((GLFWwindow*)QwerkE::ServiceLocator::UnregisterService(eEngineServices::AppWindow));

    delete (EventManager*)QwerkE::ServiceLocator::UnregisterService(eEngineServices::Event_System);

	delete m_SceneManager;

    delete (Factory*)QwerkE::ServiceLocator::UnregisterService(eEngineServices::Factory_Entity);

	delete m_Editor;

	delete (PhysicsManager*)QwerkE::ServiceLocator::UnregisterService(eEngineServices::PhysicsManager);

	delete (MessageManager*)QwerkE::ServiceLocator::UnregisterService(eEngineServices::MessageManager);

	delete (Renderer*)QwerkE::ServiceLocator::UnregisterService(eEngineServices::Renderer);

	Libs_TearDown(); // unload libraries

	// TODO: Safety checks?
	return eEngineMessage::_QSuccess;
}

Sprite2D* g_Sprite;
void Engine::Run()
{
	g_FBO->Init();
	SetupCallbacks(m_Window);

    glClearColor(1, 1, 1, 1);
	// turn on depth buffer testing
	glEnable(GL_DEPTH_TEST);

	// depth cull for efficiency
	// Testing: glEnable(GL_CULL_FACE);
	// Testing: glCullFace(GL_BACK);
	if(Wind_CCW) glFrontFace(GL_CCW);
    else glFrontFace(GL_CW);

	// turn on alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, g_WindowWidth, g_WindowHeight);

    m_SceneManager->Initialize();
    m_SceneManager->GetCurrentScene()->SetIsEnabled(true);

	ResourceManager* rMan = (ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager);
    g_Sprite = new Sprite2D(); // TEMP:
	g_Sprite->SetPosition(vec3(0,0,0));
	g_Sprite->SetMesh(rMan->GetMesh("Box"));
	g_Sprite->SetShader(rMan->GetShader("Sprite2D"));
	g_Sprite->SetTexture(rMan->GetTexture("PeriodicHeal"));

	// Deltatime + FPS Tracking //
	// Deltatime
	double deltaTime = 0.0f; // Time between current frame and last frame
	double lastFrame = helpers_Time(); // Time of last frame initialized to current time
	// Limit framerate
	int FPS_MAX = 120; // maximum number of frames that can be run be second
	float FPS_MAX_DELTA = 1.0f / FPS_MAX;
    g_TimeSinceLastFrame = FPS_MAX; // Amount of seconds since the last frame ran initialized to run 1st time
	// Printing framerate
	float printPeriod = 3.0f; // Print period in seconds
	float timeSincePrint = printPeriod; // Seconds since last print initialized to print 1st frame
	short framesSincePrint = 0;

	// Application Loop
	while (glfwWindowShouldClose(m_Window) == false) // Run until close requested
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
			Engine::NewFrame();

			/* Input */
			Engine::Input();

			/* Logic */
			Engine::Update(g_TimeSinceLastFrame);

			/* Render */
			Engine::Draw();

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
	/* Reset */
	// TODO: Reset things...
	m_Editor->NewFrame();

	ImGui_ImplGlfwGL3_NewFrame();
}

void Engine::Input()
{
	glfwPollEvents(); // TODO: Better GLFW interface?

	// g_MainWindow->CheckInput();
	// ProcessXInput();
}

void Engine::Update(double deltatime)
{
	// TODO: Update editor and/or game code
	m_SceneManager->Update(deltatime);
	m_Editor->Update();

	if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(m_Window, true);
}

void Engine::Draw()
{
	// TEMP: Render scene to texture
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	g_FBO->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_SceneManager->Draw();
	g_FBO->UnBind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui::Image(ImTextureID(g_FBO->GetTextureID()), ImVec2(320, 180));

	m_Editor->Draw();
	((Renderer*)QwerkE::ServiceLocator::GetService(eEngineServices::Renderer))->DrawFont("Text");
	// g_Sprite->Draw();
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(m_Window);
}