#include "library.h"

static void error_callback(int error, const char* description)
{
	// fprintf(stderr, "Error %d: %s\n", error, description);
}

MyGLFW::MyGLFW(const char* windowTitle, vec2 windowSize, vec2 windowPos)
{
	// Setup window
	glfwSetErrorCallback(error_callback); // console print function
	assert(glfwInit());

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core == programmable pipeline
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // user can resize window (windowed)
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE); // show window at startup (windowed)
	glfwWindowHint(GLFW_FOCUSED, GL_TRUE); // receive input focus at startup (windowed)
	glfwWindowHint(GLFW_AUTO_ICONIFY, GL_TRUE); // restore previous video mode on focus loss (fullscreen)
	glfwWindowHint(GLFW_DECORATED, GL_FALSE); // show windows title bar (icon, title, minimize, maximize, close) (windowed)
	glfwWindowHint(GLFW_FLOATING, GL_FALSE); // float above all other windows (windowed)
	glfwWindowHint(GLFW_MAXIMIZED, GL_FALSE); // open window at local monitor resolution (windowed)

#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	m_Window = glfwCreateWindow((int)windowSize.x, (int)windowSize.y, windowTitle, NULL, NULL);
	if (m_Window == nullptr)
	{
		// failed
		std::cout << "Failed to create MyGLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_Window); // set priority + focus

	// init glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		// ConsolePrint();
		std::cout << "Failed to initialize GLEW!" << std::endl;
	}
	//glfwSwapInterval(); // toggle v-sync

	glfwSetWindowPos(m_Window, (int)windowPos.x, (int)windowPos.y); // initial window position
	std::cout << "GLFW + GLEW Success!" << std::endl;
}

MyGLFW::~MyGLFW()
{
	glfwTerminate();
}
// clear screen
void MyGLFW::Clear()
{
	// screen
	glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);
	// buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
// INPUT
void MyGLFW::SetInputMode()
{
	// glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
// input callbacks
void MyGLFW::SetKeyCallback(GLFWkeyfun key_callback) // callback
{
	glfwSetKeyCallback(m_Window, key_callback); // set keyboard input callback function
}
void MyGLFW::SetCharCallback(GLFWcharfun char_callback) // callback
{
	glfwSetCharCallback(m_Window, char_callback); // set char input callback function
}
void MyGLFW::SetCharModsCallback(GLFWcharmodsfun char_mods_callback) // callback
{
	glfwSetCharModsCallback(m_Window, char_mods_callback); // set mouse button input callback function
}
void MyGLFW::SetCursorMoveCallback(GLFWcursorposfun cursorPos_callback) // callback
{
	glfwSetCursorPosCallback(m_Window, cursorPos_callback); // set cursor move callback function
}
void MyGLFW::SetScrollCallback(GLFWscrollfun scroll_callback) // callback
{
	glfwSetScrollCallback(m_Window, scroll_callback); // set mouse scroll input callback function
}
void MyGLFW::SetMouseButtonCallback(GLFWmousebuttonfun mouse_button_callback) // callback
{
	glfwSetMouseButtonCallback(m_Window, mouse_button_callback); // set mouse button input callback function
}
// window callbacks
void MyGLFW::SetWindowMoveCallback(GLFWwindowposfun window_position_callback)
{
	glfwSetWindowPosCallback(m_Window, window_position_callback); // window re-positioned
}
void MyGLFW::SetWindowResizeCallback(GLFWwindowsizefun window_resize_callback)
{
	glfwSetWindowSizeCallback(m_Window, window_resize_callback); // window resized
}
void MyGLFW::SetWindowCloseCallback(GLFWwindowclosefun window_close_callback)
{
	glfwSetWindowCloseCallback(m_Window, window_close_callback); // window requested close
}
void MyGLFW::SetWindowRefreshCallback(GLFWwindowrefreshfun window_refresh_callback)
{
	glfwSetWindowRefreshCallback(m_Window, window_refresh_callback); // window refreshed
}
void MyGLFW::SetWindowFocusCallback(GLFWwindowfocusfun window_focus_callback)
{
	glfwSetWindowFocusCallback(m_Window, window_focus_callback); // window changed focus
}
void MyGLFW::SetWindowIconifyCallback(GLFWwindowiconifyfun window_iconify_callback)
{
	glfwSetWindowIconifyCallback(m_Window, window_iconify_callback); // window iconified
}
void MyGLFW::SetFrameBufferResizeCallback(GLFWframebuffersizefun frame_buffer_callback)
{
	glfwSetFramebufferSizeCallback(m_Window, frame_buffer_callback); // frame buffer resize
}
// error callback
void MyGLFW::SetErrorCallback(GLFWerrorfun error_callback)
{
	glfwSetErrorCallback(error_callback);
}
void MyGLFW::SetWindowPosition(int x, int y)
{
	glfwSetWindowPos(m_Window, x, y);
}