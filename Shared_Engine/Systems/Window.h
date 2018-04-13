#ifndef _Window_H_
#define _Window_H_

#include "../Math/Vector.h"
#include "../../Shared_Generic/Math_Includes.h"

class GLFWwindow;

// TODO: Create windows for different libraries
class Window
{
public:
	Window(int windowWidth, int windowHeight, const char* windowTitle);
	~Window();

	void SetResolution(vec2 resolution) { m_Resolution = resolution; }
	vec2 GetResolution() { return m_Resolution; }

	void* GetWindow() { return m_Window; }

private:
	vec2 m_Resolution = vec2(16, 9);

#ifdef _glfw3_h_
	GLFWwindow* m_Window = nullptr;
#endif // !_glfw3_h_
};

#endif // !_Window_H_
