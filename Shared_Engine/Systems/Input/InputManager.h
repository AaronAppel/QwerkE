#ifndef __InputManager_H_
#define __InputManager_H_

#include "QwerKE_eKeys.h"

class Controller;

const int g_NumPlayers = 1;

// TODO: Create GLFW callback functions to link for GLFW projects
// TODO: Add input functions for non-GLFW projects
// TODO: Add input query functions such as WNDPROC for getting input and CheckInput() game loop function.

class InputManager
{
public:
	InputManager();
	~InputManager();

	// Controller* GetController(int controllerIndex) { return m_Controllers[controllerIndex]; };

	void ProcessKeyEvent(eKeys key, eKeyState state);

	eKeys GetKeyCode(int key); // TOOD: Better way for public access to m_KeyCode?
	
	// check if keyIsDown
	bool GetIsKeyDown(eKeys key)
	{
		return m_KeyStates[key];
	}

private:
	unsigned short* m_KeyCodex; // [eKeys_MAX] = { 0 }; // initialize to 0
	bool m_KeyStates[eKeys_MAX] = { false }; // initialize to false

	// Setup system key maps
	void AssignGLFWKeys();
	void AssignWin32Keys();
	void AssignMacKeys() {} // TODO:
	void AssignLinuxKeys() {} // TODO:

#ifdef _glfw3_h_
	void AssignSystemKeys()
	{
		// TODO: Reduce the size of array
		m_KeyCodex = new unsigned short[GLFW_KEY_LAST]; // number of recognized glfw keys
		AssignGLFWKeys();
	}
#elif _QWin32
	void AssignSystemKeys()
	{
		AssignWin32Keys();
	}
#elif _QMac32 // 64
	void AssignSystemKeys()
	{
		AssignMacKeys();
	}
#else
	// Other Platforms or default layout
	void AssignSystemKeys()
	{
		AssignLinuxKeys();
	}
#endif
	// Controller* m_Controllers[g_NumPlayers];
	void SetupControllers();
};

#endif //__InputManager_H_