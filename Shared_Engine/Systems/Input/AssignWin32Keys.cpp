#include "InputManager.h"
#include <Windows.h>

// Windows 32 bit
void InputManager::AssignWin32Keys()
{
	/* Generic */
	// Mouse
	m_Keys[eKeys_LeftClick] = VK_LBUTTON;
	m_Keys[eKeys_RightClick] = VK_RBUTTON;
	m_Keys[eKeys_MiddleClick] = VK_MBUTTON;
	m_Keys[eKeys_Mouse_Last];

	m_Keys[eKeys_MouseButton1] = VK_XBUTTON1;
	m_Keys[eKeys_MouseButton2] = VK_XBUTTON2;

	// Modifiers
	// Either
	m_Keys[eKeys_CTRL] = VK_CONTROL;
	m_Keys[eKeys_SHIFT] = VK_SHIFT;
	m_Keys[eKeys_ALT] = VK_MENU;
	// Left
	m_Keys[eKeys_LCTRL] = VK_LCONTROL;
	m_Keys[eKeys_LSHIFT] = VK_LSHIFT;
	m_Keys[eKeys_LALT] = VK_MENU; // TODO: Is there L/R Alt buttons?
	// Right
	m_Keys[eKeys_RCTRL] = VK_RCONTROL;
	m_Keys[eKeys_RSHIFT] = VK_RSHIFT;
	m_Keys[eKeys_RALT] = VK_MENU;

	// Alternate Keys
	m_Keys[eKeys_Return] = VK_RETURN;
	m_Keys[eKeys_Insert] = VK_INSERT;
	m_Keys[eKeys_Delete] = VK_DELETE;
	m_Keys[eKeys_PageUp] = VK_PRIOR;
	m_Keys[eKeys_PageDown] = VK_NEXT;
	m_Keys[eKeys_Home] = VK_HOME;
	m_Keys[eKeys_End] = VK_END;
	m_Keys[eKeys_Tab] = VK_TAB;
	m_Keys[eKeys_Escape] = VK_ESCAPE;

	m_Keys[eKeys_Backspace] = VK_ESCAPE;
	m_Keys[eKeys_PrintScreen] = VK_ESCAPE;
	m_Keys[eKeys_Pause] = VK_ESCAPE;
	m_Keys[eKeys_Menu] = VK_ESCAPE;
	m_Keys[eKeys_FN] = VK_ESCAPE;

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
	m_Keys[eKeys_1] = 0x31;
	m_Keys[eKeys_2] = 0x32;
	m_Keys[eKeys_3] = 0x33;
	m_Keys[eKeys_4] = 0x34;
	m_Keys[eKeys_5] = 0x35;
	m_Keys[eKeys_6] = 0x36;
	m_Keys[eKeys_7] = 0x37;
	m_Keys[eKeys_8] = 0x38;
	m_Keys[eKeys_9] = 0x39;

	// Arrow keys
	m_Keys[eKeys_Up] = VK_UP;
	m_Keys[eKeys_Down] = VK_DOWN;
	m_Keys[eKeys_Left] = VK_LEFT;
	m_Keys[eKeys_Right] = VK_RIGHT;

	// Function Keys
	m_Keys[eKeys_F1] = VK_F1;
	m_Keys[eKeys_F2] = VK_F2;
	m_Keys[eKeys_F3] = VK_F3;
	m_Keys[eKeys_F4] = VK_F4;
	m_Keys[eKeys_F5] = VK_F5;
	m_Keys[eKeys_F6] = VK_F6;
	m_Keys[eKeys_F7] = VK_F7;
	m_Keys[eKeys_F8] = VK_F8;
	m_Keys[eKeys_F9] = VK_F9;
	m_Keys[eKeys_F10] = VK_F10;
	m_Keys[eKeys_F11] = VK_F11;
	m_Keys[eKeys_F12] = VK_F12;
	m_Keys[eKeys_F13] = VK_F13;
	m_Keys[eKeys_F14] = VK_F14;
	m_Keys[eKeys_F15] = VK_F15;
	m_Keys[eKeys_F16] = VK_F16;
	m_Keys[eKeys_F17] = VK_F17;
	m_Keys[eKeys_F18] = VK_F18;
	m_Keys[eKeys_F19] = VK_F19;
	m_Keys[eKeys_F20] = VK_F20;
	m_Keys[eKeys_F21] = VK_F21;
	m_Keys[eKeys_F22] = VK_F22;
	m_Keys[eKeys_F23] = VK_F23;
	m_Keys[eKeys_F24] = VK_F24;
	m_Keys[eKeys_F25]; // Only 24 function keys on windows

	// System Keys
	m_Keys[eKeys_System] = VK_LWIN; // TODO: Also handle VK_RWIN
}