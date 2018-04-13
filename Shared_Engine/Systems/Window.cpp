#include "Window.h"
#include "../../Shared_Generic/Libraries_Include.h"

Window::Window(int windowWidth, int windowHeight, const char* windowTitle)
{
#ifdef _glfw3_h_
	m_Window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
	glfwSwapInterval(0); // TODO: Add VSynch control and toggling
	glfwMakeContextCurrent(m_Window);
	ImGui_ImplGlfwGL3_Init(m_Window, true);
#endif // !_glfw3_h_
}

Window::~Window()
{
#ifdef _glfw3_h_
	glfwDestroyWindow(m_Window);
#endif // !_glfw3_h_
}
