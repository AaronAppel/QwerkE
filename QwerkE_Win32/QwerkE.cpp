#include "QwerkE.h"

// TODO: No Globals!
int g_WindowWidth = 1280, g_WindowHeight = 720;
const char* g_WindowTitle = "Title";

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