#include "QwerkE.h"

// TODO: No Globals!
int g_WindowWidth = 1280, g_WindowHeight = 720;
const char* g_WindowTitle = "Title";

int main()
{
    /* glfw */
    GLFWwindow* window;

    if (!glfwInit())
        return 0; // glfw crashed

    window = glfwCreateWindow(g_WindowWidth, g_WindowHeight, g_WindowTitle, NULL, NULL);
    if (!window)
        return 0; // glfw crashed

    glfwMakeContextCurrent(window);

    /* glew */
    if (glewInit() != GLEW_OK)
        return 0; // glew crashed

    /* opengl */
    glViewport(0, 0, g_WindowWidth , g_WindowHeight);

    /* imgui */
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfwGL3_Init(window, true);

    glfwDestroyWindow(window); // destroy window
    ImGui_ImplGlfwGL3_Shutdown(); // shutdown imgui
    ImGui::DestroyContext(); // destroy imgui
    glfwTerminate(); // shutdown glfw

    return 0;
}