#include "../QwerkE.h" // include engine
#include "Systems/Graphics/OpenGLHelpers.h"
#include "Engine_Enums.h"
#include "Utilities/PrintFunctions.h"
#include "Engine.h"

// TODO: No Globals!
int g_WindowWidth = 1280, g_WindowHeight = 720; // (1280x720)(1600x900)(1920x1080)(2560x1440)
const char* g_WindowTitle = "Title";

float g_FrameRate = 0.0f;
extern MyGLFW* g_MainWindow = nullptr;
extern GameCore* g_GameCore = nullptr;
extern const int g_NumPlayers; // Defined in InputManager.h
extern InputManager* g_InputManager = nullptr;
extern Controller* g_Player1Controller = nullptr;
extern XinputHandler* g_XinputHandler = nullptr;
extern bool g_Debugging = false;
extern double g_TimeSinceExe = 0.0f;

int main()
{
	// setup //
	/* glfw */
	GLFWwindow* window;

	if (!glfwInit())
		return 0; // glfw error

	window = glfwCreateWindow(g_WindowWidth, g_WindowHeight, g_WindowTitle, NULL, NULL);
	if (!window)
		return 0; // glfw error

	glfwMakeContextCurrent(window);

	/* glew */
	if (glewInit() != GLEW_OK)
		return 0; // glew error

	/* opengl */
	glViewport(0, 0, g_WindowWidth, g_WindowHeight);
	glClearColor(1, 1, 1, 1); // white

	/* imgui */
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, true);

	// start experimental //
	Engine engine;
	if (engine.Startup() == eEngineMessage::_QFail)
	{
		ConsolePrint("Main(): Error loading services!");
	}

	ShaderProgram shader;
	shader.Init("../Shared_Generic/Resources/Shaders/Sprite2D.vert", "../Shared_Generic/Resources/Shaders/Sprite2D.frag", NULL);

	Mesh* mesh = MeshFactory::CreateBox(vec2(1,1));

	GLuint texture;// = Load2DTexture("../Shared_Generic/Resources/Textures/FlashHeal.png", true);
	ResourceManager* resourceManager = (ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager);
	texture = resourceManager->GetTexture("FlashHeal");

	Sprite2D sprite;
	sprite.SetPosition(vec3(0,0,0));
	sprite.SetMesh(mesh);
	sprite.SetShader(&shader);
	sprite.SetTexture(texture);
	// end experimental //

	// game loop //
	while (!glfwWindowShouldClose(window))
	{
		/* input */
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		/* logic */

		/* render */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// ImGui::ShowDemoWindow();

		sprite.Draw(); // TODO: Why no draw?

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	// clean up //
	shader.CleanUp();
	delete mesh;
	glDeleteTextures(1, &texture);

	glfwDestroyWindow(window); // destroy window
	ImGui_ImplGlfwGL3_Shutdown(); // shutdown imgui
	ImGui::DestroyContext(); // destroy imgui
	glfwTerminate(); // shutdown glfw

    return 0;
}