#include "stdafx.h"

// Windows 32 bit
void InputManager::AssignWin32Keys()
{
	/* Generic */
	// Mouse
	m_Keys[eKeys_LeftClick] = VK_LBUTTON;
	m_Keys[eKeys_RightClick] = VK_RBUTTON;
	m_Keys[eKeys_MiddleClick] = VK_MBUTTON;
	m_Keys[eKeys_MouseButton1] = VK_XBUTTON1;

	// Modifiers
	m_Keys[eKeys_CTRL] = VK_CONTROL;
	m_Keys[eKeys_SHIFT] = VK_SHIFT; // VK_LSHIFT
	m_Keys[eKeys_ALT] = VK_MENU;

	// Alternate Keys
	m_Keys[eKeys_RETURN] = VK_RETURN;
	m_Keys[eKeys_ESCAPE] = VK_ESCAPE;

	// Alphabet
	m_Keys[eKeys_A] = 0x41; // ASCII "A" == 65 == 0x41
	m_Keys[eKeys_B] = 0x42;
	m_Keys[eKeys_C] = 0x43;
	m_Keys[eKeys_D] = 0x44;
	m_Keys[eKeys_E] = 0x45;
	m_Keys[eKeys_F] = 0x46;
	m_Keys[eKeys_G] = 0x47;
	m_Keys[eKeys_H] = 0x48;
	m_Keys[eKeys_I] = 0x49;
	m_Keys[eKeys_J] = 0x4A;
	m_Keys[eKeys_K] = 0x4B;
	m_Keys[eKeys_L] = 0x4C;
	m_Keys[eKeys_M] = 0x4D;
	m_Keys[eKeys_N] = 0x4E;
	m_Keys[eKeys_O] = 0x4F;
	m_Keys[eKeys_P] = 0x50;
	m_Keys[eKeys_Q] = 0x51;
	m_Keys[eKeys_R] = 0x52;
	m_Keys[eKeys_S] = 0x53;
	m_Keys[eKeys_T] = 0x54;
	m_Keys[eKeys_U] = 0x55;
	m_Keys[eKeys_V] = 0x56;
	m_Keys[eKeys_W] = 0x57;
	m_Keys[eKeys_X] = 0x58;
	m_Keys[eKeys_Y] = 0x59;
	m_Keys[eKeys_Z] = 0x5A;

	// Numbers
	m_Keys[eKeys_0] = 0x30;

	// Arrow keys
	m_Keys[eKeys_Up] = VK_UP;
	m_Keys[eKeys_Down] = VK_DOWN;
	m_Keys[eKeys_Left] = VK_LEFT;
	m_Keys[eKeys_Right] = VK_RIGHT;

	// Function Keys
	m_Keys[eKeys_F1] = VK_F1;

	// System Keys
	m_Keys[eKeys_WIN] = VK_LWIN; // VK_RWIN
}