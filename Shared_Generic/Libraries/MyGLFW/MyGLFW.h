#ifndef _MyGLFW_H_
#define _MyGLFW_H_

/* External libraries */
#include <assert.h>
#include <iostream>
#define MyAssert assert

// OpenGL
#ifndef __GLEW_H__

#ifdef MyGLFW_Export
//#define GLEW_STATIC // static .lib
#define GLEW_BUILD // .dll
#endif

#include "GL/glew.h"
#endif // !__GLEW_H__

#ifndef _glfw3_h_
#include "GLFW/glfw3.h"
#endif // !__GLEW_H__

#include "Math_Includes.h"

typedef Vector2 vec2;
typedef Vector3 vec3;
typedef Vector4 vec4;

class  MyGLFW
{
public:
	MyGLFW(const char* windowTitle, vec2 windowSize = vec2(1280, 720), vec2 windowPos = vec2(100, 100));
	~MyGLFW();

	/* Event Callbacks */
	// input
	void SetInputMode(); // TODO:: Add functionality
	void SetKeyCallback(GLFWkeyfun key_callback);
	void SetCursorMoveCallback(GLFWcursorposfun key_callback);
	void SetScrollCallback(GLFWscrollfun scroll_callback);
	void SetCharCallback(GLFWcharfun char_callback);
	void SetMouseButtonCallback(GLFWmousebuttonfun mouse_button_callback);
	void SetCharModsCallback(GLFWcharmodsfun char_mods_callback);
	// window
	void SetWindowMoveCallback(GLFWwindowposfun window_position_callback);
	void SetWindowResizeCallback(GLFWwindowsizefun window_resize_callback);
	void SetWindowCloseCallback(GLFWwindowclosefun window_close_callback);
	void SetWindowRefreshCallback(GLFWwindowrefreshfun window_refresh_callback);
	void SetWindowFocusCallback(GLFWwindowfocusfun window_focus_callback);
	void SetWindowIconifyCallback(GLFWwindowiconifyfun window_iconify_callback);
	void SetFrameBufferResizeCallback(GLFWframebuffersizefun frame_buffer_callback);
	// error
	void SetErrorCallback(GLFWerrorfun error_callback);

	void SetCursor() {};

	// Game Loop
	void CheckInput() { glfwPollEvents(); };
	void Clear();
	void SwapBuffers() { glfwSwapBuffers(m_Window); };

	void RequestClose() { glfwSetWindowShouldClose(m_Window, GL_TRUE); };

	int CloseRequested() {return glfwWindowShouldClose(m_Window); }
	GLFWwindow* GetWindow() { return m_Window; };

	void GetWindowPosition(int *x, int *y) { glfwGetWindowPos(m_Window, x, y); };
	void GetWindowSize(int *x, int *y) { glfwGetWindowSize(m_Window, x, y); }

	double GetTime() { return glfwGetTime(); };

	void SetClearColor(vec4 color) { m_ClearColor = color; };
	void SetWindowPosition(int x, int y);

private:
	GLFWwindow* m_Window = nullptr;
	vec4 m_ClearColor = vec4(0.3f, 0.3f, 0.3f, 1);
};

#endif //!_MyGLFW_H_
