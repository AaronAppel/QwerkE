#include "stdafx.h"

// Windows 32 bit
void InputManager::AssignGLFWKeys()
{
	/* Generic */
	// Mouse
	m_Keys[eKeys_LeftClick] = GLFW_MOUSE_BUTTON_LEFT;
	m_Keys[eKeys_RightClick] = GLFW_MOUSE_BUTTON_RIGHT;
	m_Keys[eKeys_MiddleClick] = GLFW_MOUSE_BUTTON_MIDDLE;
	m_Keys[eKeys_MouseButton1] = GLFW_MOUSE_BUTTON_1;

	// Modifiers
	m_Keys[eKeys_CTRL] = GLFW_KEY_LEFT_CONTROL; // GLFWK_KEY_RIGHT_CONTROL
	m_Keys[eKeys_SHIFT] = GLFW_KEY_LEFT_SHIFT; // LGW_KEY_RIGHT_SHIFT
	m_Keys[eKeys_ALT] = GLFW_KEY_LEFT_ALT; // GLFW_KEY_RIGHT_ALT

	// Alternate Keys
	m_Keys[eKeys_RETURN] = GLFW_KEY_ENTER;

	// Alphabet
	m_Keys[eKeys_A] = GLFW_KEY_A;
	m_Keys[eKeys_B] = GLFW_KEY_B;
	m_Keys[eKeys_C] = GLFW_KEY_C;
	m_Keys[eKeys_D] = GLFW_KEY_D;
	m_Keys[eKeys_E] = GLFW_KEY_E;
	m_Keys[eKeys_F] = GLFW_KEY_F;
	m_Keys[eKeys_G] = GLFW_KEY_G;
	m_Keys[eKeys_H] = GLFW_KEY_H;
	m_Keys[eKeys_I] = GLFW_KEY_I;
	m_Keys[eKeys_J] = GLFW_KEY_J;
	m_Keys[eKeys_K] = GLFW_KEY_K;
	m_Keys[eKeys_L] = GLFW_KEY_L;
	m_Keys[eKeys_M] = GLFW_KEY_M;
	m_Keys[eKeys_N] = GLFW_KEY_N;
	m_Keys[eKeys_O] = GLFW_KEY_O;
	m_Keys[eKeys_P] = GLFW_KEY_P;
	m_Keys[eKeys_Q] = GLFW_KEY_Q;
	m_Keys[eKeys_R] = GLFW_KEY_R;
	m_Keys[eKeys_S] = GLFW_KEY_S;
	m_Keys[eKeys_T] = GLFW_KEY_T;
	m_Keys[eKeys_U] = GLFW_KEY_U;
	m_Keys[eKeys_V] = GLFW_KEY_V;
	m_Keys[eKeys_W] = GLFW_KEY_W;
	m_Keys[eKeys_X] = GLFW_KEY_X;
	m_Keys[eKeys_Y] = GLFW_KEY_Y;
	m_Keys[eKeys_Z] = GLFW_KEY_Z;

	// Numbers
	m_Keys[eKeys_0] = GLFW_KEY_0;

	// Arrow keys
	m_Keys[eKeys_Up] = GLFW_KEY_UP;
	m_Keys[eKeys_Down] = GLFW_KEY_DOWN;
	m_Keys[eKeys_Left] = GLFW_KEY_LEFT;
	m_Keys[eKeys_Right] = GLFW_KEY_RIGHT;

	// Function Keys
	m_Keys[eKeys_F1] = GLFW_KEY_F1;

	// System Keys
	m_Keys[eKeys_WIN] = -1;
}