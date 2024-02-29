#include "QF_Input.h"

#include "Libraries/glfw/glfw3.h"

#include "QF_Defines.h"

namespace QwerkE {

    namespace Input {

#ifdef GLFW3
        void InitializeGLFWKeysCodex(u16* keysCodex)
        {
            // keysCodex[?] = eKeys_CTRL; // TODO: Handle either left Or right
            // s_keysCodex[?] = eKeys_SHIFT; // TODO: Handle either left Or right
            // s_keysCodex[?] = eKeys_ALT; // TODO: Handle either left Or right

            keysCodex[GLFW_KEY_LEFT_CONTROL] = eKeys_LCTRL;
            keysCodex[GLFW_KEY_LEFT_SHIFT] = eKeys_LSHIFT;
            keysCodex[GLFW_KEY_LEFT_ALT] = eKeys_LALT;
            keysCodex[GLFW_KEY_LEFT_SUPER] = eKeys_LSUPER;

            keysCodex[GLFW_KEY_RIGHT_CONTROL] = eKeys_RCTRL;
            keysCodex[GLFW_KEY_RIGHT_SHIFT] = eKeys_RSHIFT;
            keysCodex[GLFW_KEY_RIGHT_ALT] = eKeys_RALT;
            keysCodex[GLFW_KEY_RIGHT_SUPER] = eKeys_RSUPER;

            keysCodex[GLFW_KEY_ENTER] = eKeys_Return;
            keysCodex[GLFW_KEY_INSERT] = eKeys_Insert;
            keysCodex[GLFW_KEY_DELETE] = eKeys_Delete;
            keysCodex[GLFW_KEY_PAGE_UP] = eKeys_PageUp;
            keysCodex[GLFW_KEY_PAGE_DOWN] = eKeys_PageDown;
            keysCodex[GLFW_KEY_HOME] = eKeys_Home;
            keysCodex[GLFW_KEY_END] = eKeys_End;
            keysCodex[GLFW_KEY_TAB] = eKeys_Tab;
            keysCodex[GLFW_KEY_ESCAPE] = eKeys_Escape;
            keysCodex[GLFW_KEY_BACKSPACE] = eKeys_Backspace;
            keysCodex[GLFW_KEY_PRINT_SCREEN] = eKeys_PrintScreen;
            keysCodex[GLFW_KEY_PAUSE] = eKeys_Pause;
            keysCodex[GLFW_KEY_MENU] = eKeys_Menu;
            // keysCodex[?] = eKeys_FN; // TODO: Find fn key
            // GLFW_KEY_RIGHT_SUPER

            keysCodex[GLFW_KEY_CAPS_LOCK] = eKeys_CapsLock,
            keysCodex[GLFW_KEY_NUM_LOCK] = eKeys_NumLock;
            keysCodex[GLFW_KEY_SCROLL_LOCK] = eKeys_ScrollLock;

            keysCodex[GLFW_KEY_KP_DECIMAL] = eKeys_Pad_Decimal;
            keysCodex[GLFW_KEY_ENTER] = eKeys_Pad_Return;
            keysCodex[GLFW_KEY_KP_ADD] = eKeys_Pad_Plus;
            keysCodex[GLFW_KEY_KP_SUBTRACT] = eKeys_Pad_Minus;
            keysCodex[GLFW_KEY_KP_DIVIDE] = eKeys_Pad_Division;
            keysCodex[GLFW_KEY_KP_MULTIPLY] = eKeys_Pad_Multiplication;
            keysCodex[GLFW_KEY_KP_EQUAL] = eKeys_Pad_Equal;

            keysCodex[GLFW_KEY_KP_0] = eKeys_Pad_0;
            keysCodex[GLFW_KEY_KP_1] = eKeys_Pad_1;
            keysCodex[GLFW_KEY_KP_2] = eKeys_Pad_2;
            keysCodex[GLFW_KEY_KP_3] = eKeys_Pad_3;
            keysCodex[GLFW_KEY_KP_4] = eKeys_Pad_4;
            keysCodex[GLFW_KEY_KP_5] = eKeys_Pad_5;
            keysCodex[GLFW_KEY_KP_6] = eKeys_Pad_6;
            keysCodex[GLFW_KEY_KP_7] = eKeys_Pad_7;
            keysCodex[GLFW_KEY_KP_8] = eKeys_Pad_8;
            keysCodex[GLFW_KEY_KP_9] = eKeys_Pad_9;

            keysCodex[GLFW_KEY_A] = eKeys_A;
            keysCodex[GLFW_KEY_B] = eKeys_B;
            keysCodex[GLFW_KEY_C] = eKeys_C;
            keysCodex[GLFW_KEY_D] = eKeys_D;
            keysCodex[GLFW_KEY_E] = eKeys_E;
            keysCodex[GLFW_KEY_F] = eKeys_F;
            keysCodex[GLFW_KEY_G] = eKeys_G;
            keysCodex[GLFW_KEY_H] = eKeys_H;
            keysCodex[GLFW_KEY_I] = eKeys_I;
            keysCodex[GLFW_KEY_J] = eKeys_J;
            keysCodex[GLFW_KEY_K] = eKeys_K;
            keysCodex[GLFW_KEY_L] = eKeys_L;
            keysCodex[GLFW_KEY_M] = eKeys_M;
            keysCodex[GLFW_KEY_N] = eKeys_N;
            keysCodex[GLFW_KEY_O] = eKeys_O;
            keysCodex[GLFW_KEY_P] = eKeys_P;
            keysCodex[GLFW_KEY_Q] = eKeys_Q;
            keysCodex[GLFW_KEY_R] = eKeys_R;
            keysCodex[GLFW_KEY_S] = eKeys_S;
            keysCodex[GLFW_KEY_T] = eKeys_T;
            keysCodex[GLFW_KEY_U] = eKeys_U;
            keysCodex[GLFW_KEY_V] = eKeys_V;
            keysCodex[GLFW_KEY_W] = eKeys_W;
            keysCodex[GLFW_KEY_X] = eKeys_X;
            keysCodex[GLFW_KEY_Y] = eKeys_Y;
            keysCodex[GLFW_KEY_Z] = eKeys_Z;

            keysCodex[GLFW_KEY_0] = eKeys_0;
            keysCodex[GLFW_KEY_1] = eKeys_1;
            keysCodex[GLFW_KEY_2] = eKeys_2;
            keysCodex[GLFW_KEY_3] = eKeys_3;
            keysCodex[GLFW_KEY_4] = eKeys_4;
            keysCodex[GLFW_KEY_5] = eKeys_5;
            keysCodex[GLFW_KEY_6] = eKeys_6;
            keysCodex[GLFW_KEY_7] = eKeys_7;
            keysCodex[GLFW_KEY_8] = eKeys_8;
            keysCodex[GLFW_KEY_9] = eKeys_9;

            keysCodex[GLFW_KEY_SPACE] = eKeys_Space;
            keysCodex[GLFW_KEY_APOSTROPHE] = eKeys_Apostrophe;
            keysCodex[GLFW_KEY_COMMA] = eKeys_Comma;
            keysCodex[GLFW_KEY_PERIOD] = eKeys_Period;
            keysCodex[GLFW_KEY_SLASH] = eKeys_ForwardSlash;
            keysCodex[GLFW_KEY_BACKSLASH] = eKeys_BackSlash;
            keysCodex[GLFW_KEY_SEMICOLON] = eKeys_SemiColon;
            keysCodex[GLFW_KEY_LEFT_BRACKET] = eKeys_LeftBracket;
            keysCodex[GLFW_KEY_RIGHT_BRACKET] = eKeys_RightBracket;
            keysCodex[GLFW_KEY_GRAVE_ACCENT] = eKeys_Tilde;
            keysCodex[GLFW_KEY_MINUS] = eKeys_Minus;
            keysCodex[GLFW_KEY_EQUAL] = eKeys_Equal;

            keysCodex[GLFW_KEY_UP] = eKeys_Up;
            keysCodex[GLFW_KEY_DOWN] = eKeys_Down;
            keysCodex[GLFW_KEY_LEFT] = eKeys_Left;
            keysCodex[GLFW_KEY_RIGHT] = eKeys_Right;

            keysCodex[GLFW_KEY_F1] = eKeys_F1;
            keysCodex[GLFW_KEY_F2] = eKeys_F2;
            keysCodex[GLFW_KEY_F3] = eKeys_F3;
            keysCodex[GLFW_KEY_F4] = eKeys_F4;
            keysCodex[GLFW_KEY_F5] = eKeys_F5;
            keysCodex[GLFW_KEY_F6] = eKeys_F6;
            keysCodex[GLFW_KEY_F7] = eKeys_F7;
            keysCodex[GLFW_KEY_F8] = eKeys_F8;
            keysCodex[GLFW_KEY_F9] = eKeys_F9;
            keysCodex[GLFW_KEY_F10] = eKeys_F10;
            keysCodex[GLFW_KEY_F11] = eKeys_F11;
            keysCodex[GLFW_KEY_F12] = eKeys_F12;

            keysCodex[GLFW_KEY_F13] = eKeys_F13;
            keysCodex[GLFW_KEY_F14] = eKeys_F14;
            keysCodex[GLFW_KEY_F15] = eKeys_F15;
            keysCodex[GLFW_KEY_F16] = eKeys_F16;
            keysCodex[GLFW_KEY_F17] = eKeys_F17;
            keysCodex[GLFW_KEY_F18] = eKeys_F18;
            keysCodex[GLFW_KEY_F19] = eKeys_F19;
            keysCodex[GLFW_KEY_F20] = eKeys_F20;
            keysCodex[GLFW_KEY_F21] = eKeys_F21;
            keysCodex[GLFW_KEY_F22] = eKeys_F22;
            keysCodex[GLFW_KEY_F23] = eKeys_F23;
            keysCodex[GLFW_KEY_F24] = eKeys_F24;
            keysCodex[GLFW_KEY_F25] = eKeys_F25;

            keysCodex[GLFW_MOUSE_BUTTON_LEFT] = eKeys_LeftClick;
            keysCodex[GLFW_MOUSE_BUTTON_RIGHT] = eKeys_RightClick;
            keysCodex[GLFW_MOUSE_BUTTON_MIDDLE] = eKeys_MiddleClick;

            keysCodex[GLFW_MOUSE_BUTTON_2] = eKeys_MouseButton2;
            keysCodex[GLFW_MOUSE_BUTTON_3] = eKeys_MouseButton3;
            keysCodex[GLFW_MOUSE_BUTTON_4] = eKeys_MouseButton4;
            keysCodex[GLFW_MOUSE_BUTTON_5] = eKeys_MouseButton5;
            keysCodex[GLFW_MOUSE_BUTTON_6] = eKeys_MouseButton6;
            keysCodex[GLFW_MOUSE_BUTTON_7] = eKeys_MouseButton7;
            keysCodex[GLFW_MOUSE_BUTTON_8] = eKeys_MouseButton8;

            // #Unsupported
            // #define GLFW_KEY_WORLD_1            161 /* non-US #1 */
            // #define GLFW_KEY_WORLD_2            162 /* non-US #2 */
            // #define GLFW_JOYSTICK_1             0, 1, 2, 3...15

        }
#endif

    }

}
