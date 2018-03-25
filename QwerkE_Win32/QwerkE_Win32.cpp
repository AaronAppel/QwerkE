#include "../QwerkE.h" // include engine

// TODO: No Globals!
int g_WindowWidth = 1280, g_WindowHeight = 720; // (1280x720)(1600x900)(1920x1080)(2560x1440)
const char* g_WindowTitle = "Title";

float g_FrameRate = 0.0f;
extern MyGLFW* g_MainWindow = nullptr;
extern GameCore* g_GameCore = nullptr;
extern const int g_NumPlayers; // InputManager.h
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

	// game loop //
	while (!glfwWindowShouldClose(window))
	{
		/* input */
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		/* logic */
		ImGui::ShowDemoWindow();

		/* render */
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	// clean up //
	glfwDestroyWindow(window); // destroy window
	ImGui_ImplGlfwGL3_Shutdown(); // shutdown imgui
	ImGui::DestroyContext(); // destroy imgui
	glfwTerminate(); // shutdown glfw

    return 0;
}