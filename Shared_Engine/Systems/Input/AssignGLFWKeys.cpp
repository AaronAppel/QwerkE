#include "InputManager.h"
#include "Libraries/glfw/GLFW/glfw3.h"

// Windows 32 bit
void InputManager::AssignGLFWKeys()
{
	/* Generic */
	// Mouse
	m_Keys[eKeys_LeftClick] = GLFW_MOUSE_BUTTON_LEFT;
	m_Keys[eKeys_RightClick] = GLFW_MOUSE_BUTTON_RIGHT;
	m_Keys[eKeys_MiddleClick] = GLFW_MOUSE_BUTTON_MIDDLE;

	m_Keys[eKeys_MouseButton1] = GLFW_MOUSE_BUTTON_1;
	m_Keys[eKeys_MouseButton2] = GLFW_MOUSE_BUTTON_2;
	m_Keys[eKeys_MouseButton3] = GLFW_MOUSE_BUTTON_3;
	m_Keys[eKeys_MouseButton4] = GLFW_MOUSE_BUTTON_4;
	m_Keys[eKeys_MouseButton5] = GLFW_MOUSE_BUTTON_5;
	m_Keys[eKeys_MouseButton6] = GLFW_MOUSE_BUTTON_6;
	m_Keys[eKeys_MouseButton7] = GLFW_MOUSE_BUTTON_7;
	m_Keys[eKeys_MouseButton8] = GLFW_MOUSE_BUTTON_8;

	// Modifiers
	// Either
	m_Keys[eKeys_CTRL]; // TODO: Handle either left Or right
	m_Keys[eKeys_SHIFT]; // TODO: Handle either left Or right
	m_Keys[eKeys_ALT]; // TODO: Handle either left Or right
	// Left
	m_Keys[eKeys_LCTRL] = GLFW_KEY_LEFT_CONTROL; // GLFWK_KEY_RIGHT_CONTROL
	m_Keys[eKeys_LSHIFT] = GLFW_KEY_LEFT_SHIFT; // LGW_KEY_RIGHT_SHIFT
	m_Keys[eKeys_LALT] = GLFW_KEY_LEFT_ALT; // GLFW_KEY_RIGHT_ALT
	// Right
	m_Keys[eKeys_RCTRL] = GLFW_KEY_RIGHT_CONTROL; // GLFWK_KEY_RIGHT_CONTROL
	m_Keys[eKeys_RSHIFT] = GLFW_KEY_RIGHT_SHIFT; // LGW_KEY_RIGHT_SHIFT
	m_Keys[eKeys_RALT] = GLFW_KEY_RIGHT_ALT; // GLFW_KEY_RIGHT_ALT

	// Alternate Keys
	m_Keys[eKeys_Return] = GLFW_KEY_ENTER;
	m_Keys[eKeys_Insert] = GLFW_KEY_INSERT;
	m_Keys[eKeys_Delete] = GLFW_KEY_DELETE;
	m_Keys[eKeys_PageUp] = GLFW_KEY_PAGE_UP;
	m_Keys[eKeys_PageDown] = GLFW_KEY_PAGE_DOWN;
	m_Keys[eKeys_Home] = GLFW_KEY_HOME;
	m_Keys[eKeys_End] = GLFW_KEY_END;
	m_Keys[eKeys_Tab] = GLFW_KEY_TAB;
	m_Keys[eKeys_Escape] = GLFW_KEY_ESCAPE;
	m_Keys[eKeys_Backspace] = GLFW_KEY_BACKSPACE;
	m_Keys[eKeys_PrintScreen] = GLFW_KEY_PRINT_SCREEN;
	m_Keys[eKeys_Pause] = GLFW_KEY_PAUSE;
	m_Keys[eKeys_Menu] = GLFW_KEY_MENU;
	m_Keys[eKeys_FN]; // TODO: Find fn key

	// Locks
	m_Keys[eKeys_CapsLock] = GLFW_KEY_CAPS_LOCK,
	m_Keys[eKeys_NumLock] = GLFW_KEY_NUM_LOCK;
	m_Keys[eKeys_ScrollLock] = GLFW_KEY_SCROLL_LOCK;

	// Key Pad
	m_Keys[eKeys_Pad_Decimal] = GLFW_KEY_KP_DECIMAL;
	m_Keys[eKeys_Pad_Return] = GLFW_KEY_ENTER;
	m_Keys[eKeys_Pad_Plus] = GLFW_KEY_KP_ADD;
	m_Keys[eKeys_Pad_Minus] = GLFW_KEY_KP_SUBTRACT;
	m_Keys[eKeys_Pad_Division] = GLFW_KEY_KP_DIVIDE;
	m_Keys[eKeys_Pad_Multiplication] = GLFW_KEY_KP_MULTIPLY;

	m_Keys[eKeys_Pad_0] = GLFW_KEY_KP_0;
	m_Keys[eKeys_Pad_1] = GLFW_KEY_KP_1;
	m_Keys[eKeys_Pad_2] = GLFW_KEY_KP_2;
	m_Keys[eKeys_Pad_3] = GLFW_KEY_KP_3;
	m_Keys[eKeys_Pad_4] = GLFW_KEY_KP_4;
	m_Keys[eKeys_Pad_5] = GLFW_KEY_KP_5;
	m_Keys[eKeys_Pad_6] = GLFW_KEY_KP_6;
	m_Keys[eKeys_Pad_7] = GLFW_KEY_KP_7;
	m_Keys[eKeys_Pad_8] = GLFW_KEY_KP_8;
	m_Keys[eKeys_Pad_9] = GLFW_KEY_KP_9;

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
	m_Keys[eKeys_1] = GLFW_KEY_1;
	m_Keys[eKeys_2] = GLFW_KEY_2;
	m_Keys[eKeys_3] = GLFW_KEY_3;
	m_Keys[eKeys_4] = GLFW_KEY_4;
	m_Keys[eKeys_5] = GLFW_KEY_5;
	m_Keys[eKeys_6] = GLFW_KEY_6;
	m_Keys[eKeys_7] = GLFW_KEY_7;
	m_Keys[eKeys_8] = GLFW_KEY_8;
	m_Keys[eKeys_9] = GLFW_KEY_9;

	// Punctuation
	m_Keys[eKeys_Space] = GLFW_KEY_SPACE;
	m_Keys[eKeys_Apostrophe] = GLFW_KEY_APOSTROPHE;
	m_Keys[eKeys_Comma] = GLFW_KEY_COMMA;
	m_Keys[eKeys_Period] = GLFW_KEY_PERIOD;
	m_Keys[eKeys_ForwardSlash] = GLFW_KEY_SLASH;
	m_Keys[eKeys_BackSlash] = GLFW_KEY_BACKSLASH;
	m_Keys[eKeys_SemiColon] = GLFW_KEY_SEMICOLON;
	m_Keys[eKeys_LeftBracket] = GLFW_KEY_LEFT_BRACKET;
	m_Keys[eKeys_RightBracket] = GLFW_KEY_RIGHT_BRACKET;
	m_Keys[eKeys_Tilde] = GLFW_KEY_GRAVE_ACCENT;
	m_Keys[eKeys_Minus] = GLFW_KEY_MINUS;
	m_Keys[eKeys_Equal] = GLFW_KEY_EQUAL;

	// Arrow keys
	m_Keys[eKeys_Up] = GLFW_KEY_UP;
	m_Keys[eKeys_Down] = GLFW_KEY_DOWN;
	m_Keys[eKeys_Left] = GLFW_KEY_LEFT;
	m_Keys[eKeys_Right] = GLFW_KEY_RIGHT;

	// Function Keys
	m_Keys[eKeys_F1] = GLFW_KEY_F1;
	m_Keys[eKeys_F2] = GLFW_KEY_F2;
	m_Keys[eKeys_F3] = GLFW_KEY_F3;
	m_Keys[eKeys_F4] = GLFW_KEY_F4;
	m_Keys[eKeys_F5] = GLFW_KEY_F5;
	m_Keys[eKeys_F6] = GLFW_KEY_F6;
	m_Keys[eKeys_F7] = GLFW_KEY_F7;
	m_Keys[eKeys_F8] = GLFW_KEY_F8;
	m_Keys[eKeys_F9] = GLFW_KEY_F9;
	m_Keys[eKeys_F10] = GLFW_KEY_F10;

	m_Keys[eKeys_F11] = GLFW_KEY_F11;
	m_Keys[eKeys_F12] = GLFW_KEY_F12;
	m_Keys[eKeys_F13] = GLFW_KEY_F13;
	m_Keys[eKeys_F14] = GLFW_KEY_F14;
	m_Keys[eKeys_F15] = GLFW_KEY_F15;
	m_Keys[eKeys_F16] = GLFW_KEY_F16;
	m_Keys[eKeys_F17] = GLFW_KEY_F17;
	m_Keys[eKeys_F18] = GLFW_KEY_F18;
	m_Keys[eKeys_F19] = GLFW_KEY_F19;
	m_Keys[eKeys_F20] = GLFW_KEY_F20;

	m_Keys[eKeys_F21] = GLFW_KEY_F21;
	m_Keys[eKeys_F22] = GLFW_KEY_F22;
	m_Keys[eKeys_F23] = GLFW_KEY_F23;
	m_Keys[eKeys_F24] = GLFW_KEY_F24;
	m_Keys[eKeys_F25] = GLFW_KEY_F25;

	// System Keys
	m_Keys[eKeys_System] = -1; // Set to Max
}