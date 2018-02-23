#ifndef __InputManager_H_
#define __InputManager_H_

enum eKeys
{
	eKeys_MIN = 0,

	// Mouse
	eKeys_RightClick,
	eKeys_LeftClick,
	eKeys_MiddleClick,
	eKeys_MouseButton1, // TODO: Add more mouse buttons

	// Modifiers
	eKeys_CTRL,
	eKeys_SHIFT,
	eKeys_ALT,

	// Alternate Keys
	eKeys_RETURN,
	eKeys_ESCAPE,

	// Alphabet
	eKeys_A,
	eKeys_B,
	eKeys_C,
	eKeys_D,
	eKeys_E,
	eKeys_F,
	eKeys_G,
	eKeys_H,
	eKeys_I,
	eKeys_J,
	eKeys_K,
	eKeys_L,
	eKeys_M,
	eKeys_N,
	eKeys_O,
	eKeys_P,
	eKeys_Q,
	eKeys_R,
	eKeys_S,
	eKeys_T,
	eKeys_U,
	eKeys_V,
	eKeys_W,
	eKeys_X,
	eKeys_Y,
	eKeys_Z,

	// Numbers
	eKeys_0,

	// Arrow keys
	eKeys_Up,
	eKeys_Down,
	eKeys_Left,
	eKeys_Right,

	// Function Keys
	eKeys_F1,

	// System Keys
	eKeys_WIN,
	eKeys_MAC,

	eKeys_MAX
};

const int g_NumPlayers = 1;

// TODO: Create GLFW callback functions to link for GLFW projects
// TODO: Add input functions for non-GLFW projects
// TODO: Add input query functions such as WNDPROC for getting input and CheckInput() game loop function.

class InputManager
{
public:
	InputManager();
	~InputManager();

	Controller* GetController(int controllerIndex) { return m_Controllers[controllerIndex]; };

private:
	void AssignGLFWKeys();
	void AssignWin32Keys();

	Controller* m_Controllers[g_NumPlayers];
	void SetupControllers();

	unsigned short m_Keys[eKeys_MAX] = {0}; // initialize to 0
};

#endif //__InputManager_H_