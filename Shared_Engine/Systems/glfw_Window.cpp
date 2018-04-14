#include "glfw_Window.h"
#include "../CallbackFunctions.h"

void close_callback(GLFWwindow* window);

glfw_Window::glfw_Window(int windowWidth, int windowHeight, const char* windowTitle) : Window(windowWidth, windowHeight, windowTitle)
{
    m_Window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
    assert(m_Window);
    glfwSetWindowCloseCallback(m_Window, close_callback);
    glfwSwapInterval(0); // TODO: Add VSynch control and toggling
    glfwMakeContextCurrent(m_Window);
    ImGui_ImplGlfwGL3_Init(m_Window, true);
    m_IsFocused = true;
    SetupCallbacks(m_Window);
}

glfw_Window::~glfw_Window()
{
    glfwWindowShouldClose(m_Window); // May have issues waiting so long to close m_Window. Try to call Close() when it is requested
    glfwDestroyWindow(m_Window);
}

void close_callback(GLFWwindow* window)
{
    WindowManager* winMan = (WindowManager*)QwerkE::ServiceLocator::GetService(eEngineServices::WindowManager);
    glfw_Window* glfw_window = (glfw_Window*)winMan->GetWindow(0);
    glfw_window->SetClosing(true);
}