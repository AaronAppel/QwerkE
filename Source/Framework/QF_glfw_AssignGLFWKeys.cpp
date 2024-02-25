#include "QF_Mouse.h"

#include "Libraries/glfw/GLFW/glfw3.h"

#include "QF_Input.h"

namespace QwerkE {

    namespace Input {

        extern u16* s_KeyCodex;

        eKeys GLFWToQwerkEKey(int key)
        {
            return (eKeys)s_KeyCodex[key];
        }

        void AssignGLFWKeys(eInputDeviceTypes deviceType, u16* deviceKeysCodex)
        {
            // #TODO Look if this can be stored and loaded from data instead of hard coded

            if (deviceType > eInputDeviceTypes::Keyboard_Min &&
                deviceType < eInputDeviceTypes::Keyboard_Max)
            {
                // Modifiers
                // Either
                // deviceKeysCodex[?] = eKeys_CTRL; // TODO: Handle either left Or right
                // s_deviceKeysCodex[?] = eKeys_SHIFT; // TODO: Handle either left Or right
                // s_deviceKeysCodex[?] = eKeys_ALT; // TODO: Handle either left Or right

                deviceKeysCodex[GLFW_KEY_LEFT_CONTROL] = eKeys_LCTRL; // GLFWK_KEY_RIGHT_CONTROL
                deviceKeysCodex[GLFW_KEY_LEFT_SHIFT] = eKeys_LSHIFT; // LGW_KEY_RIGHT_SHIFT
                deviceKeysCodex[GLFW_KEY_LEFT_ALT] = eKeys_LALT; // GLFW_KEY_RIGHT_ALT

                deviceKeysCodex[GLFW_KEY_RIGHT_CONTROL] = eKeys_RCTRL; // GLFWK_KEY_RIGHT_CONTROL
                deviceKeysCodex[GLFW_KEY_RIGHT_SHIFT] = eKeys_RSHIFT; // LGW_KEY_RIGHT_SHIFT
                deviceKeysCodex[GLFW_KEY_RIGHT_ALT] = eKeys_RALT; // GLFW_KEY_RIGHT_ALT

                deviceKeysCodex[GLFW_KEY_ENTER] = eKeys_Return;
                deviceKeysCodex[GLFW_KEY_INSERT] = eKeys_Insert;
                deviceKeysCodex[GLFW_KEY_DELETE] = eKeys_Delete;
                deviceKeysCodex[GLFW_KEY_PAGE_UP] = eKeys_PageUp;
                deviceKeysCodex[GLFW_KEY_PAGE_DOWN] = eKeys_PageDown;
                deviceKeysCodex[GLFW_KEY_HOME] = eKeys_Home;
                deviceKeysCodex[GLFW_KEY_END] = eKeys_End;
                deviceKeysCodex[GLFW_KEY_TAB] = eKeys_Tab;
                deviceKeysCodex[GLFW_KEY_ESCAPE] = eKeys_Escape;
                deviceKeysCodex[GLFW_KEY_BACKSPACE] = eKeys_Backspace;
                deviceKeysCodex[GLFW_KEY_PRINT_SCREEN] = eKeys_PrintScreen;
                deviceKeysCodex[GLFW_KEY_PAUSE] = eKeys_Pause;
                deviceKeysCodex[GLFW_KEY_MENU] = eKeys_Menu;
                // deviceKeysCodex[?] = eKeys_FN; // TODO: Find fn key
                // GLFW_KEY_RIGHT_SUPER

                deviceKeysCodex[GLFW_KEY_CAPS_LOCK] = eKeys_CapsLock,
                deviceKeysCodex[GLFW_KEY_NUM_LOCK] = eKeys_NumLock;
                deviceKeysCodex[GLFW_KEY_SCROLL_LOCK] = eKeys_ScrollLock;

                deviceKeysCodex[GLFW_KEY_KP_DECIMAL] = eKeys_Pad_Decimal;
                deviceKeysCodex[GLFW_KEY_ENTER] = eKeys_Pad_Return;
                deviceKeysCodex[GLFW_KEY_KP_ADD] = eKeys_Pad_Plus;
                deviceKeysCodex[GLFW_KEY_KP_SUBTRACT] = eKeys_Pad_Minus;
                deviceKeysCodex[GLFW_KEY_KP_DIVIDE] = eKeys_Pad_Division;
                deviceKeysCodex[GLFW_KEY_KP_MULTIPLY] = eKeys_Pad_Multiplication;

                deviceKeysCodex[GLFW_KEY_KP_0] = eKeys_Pad_0;
                deviceKeysCodex[GLFW_KEY_KP_1] = eKeys_Pad_1;
                deviceKeysCodex[GLFW_KEY_KP_2] = eKeys_Pad_2;
                deviceKeysCodex[GLFW_KEY_KP_3] = eKeys_Pad_3;
                deviceKeysCodex[GLFW_KEY_KP_4] = eKeys_Pad_4;
                deviceKeysCodex[GLFW_KEY_KP_5] = eKeys_Pad_5;
                deviceKeysCodex[GLFW_KEY_KP_6] = eKeys_Pad_6;
                deviceKeysCodex[GLFW_KEY_KP_7] = eKeys_Pad_7;
                deviceKeysCodex[GLFW_KEY_KP_8] = eKeys_Pad_8;
                deviceKeysCodex[GLFW_KEY_KP_9] = eKeys_Pad_9;

                deviceKeysCodex[GLFW_KEY_A] = eKeys_A;
                deviceKeysCodex[GLFW_KEY_B] = eKeys_B;
                deviceKeysCodex[GLFW_KEY_C] = eKeys_C;
                deviceKeysCodex[GLFW_KEY_D] = eKeys_D;
                deviceKeysCodex[GLFW_KEY_E] = eKeys_E;
                deviceKeysCodex[GLFW_KEY_F] = eKeys_F;
                deviceKeysCodex[GLFW_KEY_G] = eKeys_G;
                deviceKeysCodex[GLFW_KEY_H] = eKeys_H;
                deviceKeysCodex[GLFW_KEY_I] = eKeys_I;
                deviceKeysCodex[GLFW_KEY_J] = eKeys_J;
                deviceKeysCodex[GLFW_KEY_K] = eKeys_K;
                deviceKeysCodex[GLFW_KEY_L] = eKeys_L;
                deviceKeysCodex[GLFW_KEY_M] = eKeys_M;
                deviceKeysCodex[GLFW_KEY_N] = eKeys_N;
                deviceKeysCodex[GLFW_KEY_O] = eKeys_O;
                deviceKeysCodex[GLFW_KEY_P] = eKeys_P;
                deviceKeysCodex[GLFW_KEY_Q] = eKeys_Q;
                deviceKeysCodex[GLFW_KEY_R] = eKeys_R;
                deviceKeysCodex[GLFW_KEY_S] = eKeys_S;
                deviceKeysCodex[GLFW_KEY_T] = eKeys_T;
                deviceKeysCodex[GLFW_KEY_U] = eKeys_U;
                deviceKeysCodex[GLFW_KEY_V] = eKeys_V;
                deviceKeysCodex[GLFW_KEY_W] = eKeys_W;
                deviceKeysCodex[GLFW_KEY_X] = eKeys_X;
                deviceKeysCodex[GLFW_KEY_Y] = eKeys_Y;
                deviceKeysCodex[GLFW_KEY_Z] = eKeys_Z;

                deviceKeysCodex[GLFW_KEY_0] = eKeys_0;
                deviceKeysCodex[GLFW_KEY_1] = eKeys_1;
                deviceKeysCodex[GLFW_KEY_2] = eKeys_2;
                deviceKeysCodex[GLFW_KEY_3] = eKeys_3;
                deviceKeysCodex[GLFW_KEY_4] = eKeys_4;
                deviceKeysCodex[GLFW_KEY_5] = eKeys_5;
                deviceKeysCodex[GLFW_KEY_6] = eKeys_6;
                deviceKeysCodex[GLFW_KEY_7] = eKeys_7;
                deviceKeysCodex[GLFW_KEY_8] = eKeys_8;
                deviceKeysCodex[GLFW_KEY_9] = eKeys_9;

                deviceKeysCodex[GLFW_KEY_SPACE] = eKeys_Space;
                deviceKeysCodex[GLFW_KEY_APOSTROPHE] = eKeys_Apostrophe;
                deviceKeysCodex[GLFW_KEY_COMMA] = eKeys_Comma;
                deviceKeysCodex[GLFW_KEY_PERIOD] = eKeys_Period;
                deviceKeysCodex[GLFW_KEY_SLASH] = eKeys_ForwardSlash;
                deviceKeysCodex[GLFW_KEY_BACKSLASH] = eKeys_BackSlash;
                deviceKeysCodex[GLFW_KEY_SEMICOLON] = eKeys_SemiColon;
                deviceKeysCodex[GLFW_KEY_LEFT_BRACKET] = eKeys_LeftBracket;
                deviceKeysCodex[GLFW_KEY_RIGHT_BRACKET] = eKeys_RightBracket;
                deviceKeysCodex[GLFW_KEY_GRAVE_ACCENT] = eKeys_Tilde;
                deviceKeysCodex[GLFW_KEY_MINUS] = eKeys_Minus;
                deviceKeysCodex[GLFW_KEY_EQUAL] = eKeys_Equal;

                deviceKeysCodex[GLFW_KEY_UP] = eKeys_Up;
                deviceKeysCodex[GLFW_KEY_DOWN] = eKeys_Down;
                deviceKeysCodex[GLFW_KEY_LEFT] = eKeys_Left;
                deviceKeysCodex[GLFW_KEY_RIGHT] = eKeys_Right;

                deviceKeysCodex[GLFW_KEY_F1] = eKeys_F1;
                deviceKeysCodex[GLFW_KEY_F2] = eKeys_F2;
                deviceKeysCodex[GLFW_KEY_F3] = eKeys_F3;
                deviceKeysCodex[GLFW_KEY_F4] = eKeys_F4;
                deviceKeysCodex[GLFW_KEY_F5] = eKeys_F5;
                deviceKeysCodex[GLFW_KEY_F6] = eKeys_F6;
                deviceKeysCodex[GLFW_KEY_F7] = eKeys_F7;
                deviceKeysCodex[GLFW_KEY_F8] = eKeys_F8;
                deviceKeysCodex[GLFW_KEY_F9] = eKeys_F9;
                deviceKeysCodex[GLFW_KEY_F10] = eKeys_F10;

                deviceKeysCodex[GLFW_KEY_F11] = eKeys_F11;
                deviceKeysCodex[GLFW_KEY_F12] = eKeys_F12;
                deviceKeysCodex[GLFW_KEY_F13] = eKeys_F13;
                deviceKeysCodex[GLFW_KEY_F14] = eKeys_F14;
                deviceKeysCodex[GLFW_KEY_F15] = eKeys_F15;
                deviceKeysCodex[GLFW_KEY_F16] = eKeys_F16;
                deviceKeysCodex[GLFW_KEY_F17] = eKeys_F17;
                deviceKeysCodex[GLFW_KEY_F18] = eKeys_F18;
                deviceKeysCodex[GLFW_KEY_F19] = eKeys_F19;
                deviceKeysCodex[GLFW_KEY_F20] = eKeys_F20;

                deviceKeysCodex[GLFW_KEY_F21] = eKeys_F21;
                deviceKeysCodex[GLFW_KEY_F22] = eKeys_F22;
                deviceKeysCodex[GLFW_KEY_F23] = eKeys_F23;
                deviceKeysCodex[GLFW_KEY_F24] = eKeys_F24;
                deviceKeysCodex[GLFW_KEY_F25] = eKeys_F25;

                // System Keys
                // deviceKeysCodex[eKeys_System] = -1; // Set to Max
            }
            else if (deviceType > eInputDeviceTypes::Mouse_Min && deviceType < eInputDeviceTypes::Mouse_Max)
            {
                deviceKeysCodex[GLFW_MOUSE_BUTTON_LEFT] = eKeys_LeftClick;
                deviceKeysCodex[GLFW_MOUSE_BUTTON_RIGHT] = eKeys_RightClick;
                deviceKeysCodex[GLFW_MOUSE_BUTTON_MIDDLE] = eKeys_MiddleClick;

                deviceKeysCodex[GLFW_MOUSE_BUTTON_2] = eKeys_MouseButton2;
                deviceKeysCodex[GLFW_MOUSE_BUTTON_3] = eKeys_MouseButton3;
                deviceKeysCodex[GLFW_MOUSE_BUTTON_4] = eKeys_MouseButton4;
                deviceKeysCodex[GLFW_MOUSE_BUTTON_5] = eKeys_MouseButton5;
                deviceKeysCodex[GLFW_MOUSE_BUTTON_6] = eKeys_MouseButton6;
                deviceKeysCodex[GLFW_MOUSE_BUTTON_7] = eKeys_MouseButton7;
                deviceKeysCodex[GLFW_MOUSE_BUTTON_8] = eKeys_MouseButton8;
            }
        }

        void SetupGLFWKeyCodex()
        {
            // #TODO Look if this can be stored and loaded from data instead of hard coded

            s_KeyCodex[GLFW_MOUSE_BUTTON_LEFT] = eKeys_LeftClick;
            s_KeyCodex[GLFW_MOUSE_BUTTON_RIGHT] = eKeys_RightClick;
            s_KeyCodex[GLFW_MOUSE_BUTTON_MIDDLE] = eKeys_MiddleClick;

            s_KeyCodex[GLFW_MOUSE_BUTTON_2] = eKeys_MouseButton2;
            s_KeyCodex[GLFW_MOUSE_BUTTON_3] = eKeys_MouseButton3;
            s_KeyCodex[GLFW_MOUSE_BUTTON_4] = eKeys_MouseButton4;
            s_KeyCodex[GLFW_MOUSE_BUTTON_5] = eKeys_MouseButton5;
            s_KeyCodex[GLFW_MOUSE_BUTTON_6] = eKeys_MouseButton6;
            s_KeyCodex[GLFW_MOUSE_BUTTON_7] = eKeys_MouseButton7;
            s_KeyCodex[GLFW_MOUSE_BUTTON_8] = eKeys_MouseButton8;

            // Modifiers
            // s_KeyCodex[CTRL] = eKeys_CTRL; // TODO: Handle either left Or right
            // s_KeyCodex[SHIFT] = eKeys_SHIFT; // TODO: Handle either left Or right
            // s_KeyCodex[ALT] = eKeys_ALT; // TODO: Handle either left Or right

            s_KeyCodex[GLFW_KEY_LEFT_CONTROL] = eKeys_LCTRL;
            s_KeyCodex[GLFW_KEY_LEFT_SHIFT] = eKeys_LSHIFT;
            s_KeyCodex[GLFW_KEY_LEFT_ALT] = eKeys_LALT;

            s_KeyCodex[GLFW_KEY_RIGHT_CONTROL] = eKeys_RCTRL;
            s_KeyCodex[GLFW_KEY_RIGHT_SHIFT] = eKeys_RSHIFT;
            s_KeyCodex[GLFW_KEY_RIGHT_ALT] = eKeys_RALT;

            s_KeyCodex[GLFW_KEY_ENTER] = eKeys_Return;
            s_KeyCodex[GLFW_KEY_INSERT] = eKeys_Insert;
            s_KeyCodex[GLFW_KEY_DELETE] = eKeys_Delete;
            s_KeyCodex[GLFW_KEY_PAGE_UP] = eKeys_PageUp;
            s_KeyCodex[GLFW_KEY_PAGE_DOWN] = eKeys_PageDown;
            s_KeyCodex[GLFW_KEY_HOME] = eKeys_Home;
            s_KeyCodex[GLFW_KEY_END] = eKeys_End;
            s_KeyCodex[GLFW_KEY_TAB] = eKeys_Tab;
            s_KeyCodex[GLFW_KEY_ESCAPE] = eKeys_Escape;
            s_KeyCodex[GLFW_KEY_BACKSPACE] = eKeys_Backspace;
            s_KeyCodex[GLFW_KEY_PRINT_SCREEN] = eKeys_PrintScreen;
            s_KeyCodex[GLFW_KEY_PAUSE] = eKeys_Pause;
            s_KeyCodex[GLFW_KEY_MENU] = eKeys_Menu;
            // s_KeyCodex[?] = eKeys_FN; // TODO: Find fn key

            s_KeyCodex[GLFW_KEY_CAPS_LOCK] = eKeys_CapsLock,
            s_KeyCodex[GLFW_KEY_NUM_LOCK] = eKeys_NumLock;
            s_KeyCodex[GLFW_KEY_SCROLL_LOCK] = eKeys_ScrollLock;

            s_KeyCodex[GLFW_KEY_KP_DECIMAL] = eKeys_Pad_Decimal;
            s_KeyCodex[GLFW_KEY_ENTER] = eKeys_Pad_Return;
            s_KeyCodex[GLFW_KEY_KP_ADD] = eKeys_Pad_Plus;
            s_KeyCodex[GLFW_KEY_KP_SUBTRACT] = eKeys_Pad_Minus;
            s_KeyCodex[GLFW_KEY_KP_DIVIDE] = eKeys_Pad_Division;
            s_KeyCodex[GLFW_KEY_KP_MULTIPLY] = eKeys_Pad_Multiplication;

            s_KeyCodex[GLFW_KEY_KP_0] = eKeys_Pad_0;
            s_KeyCodex[GLFW_KEY_KP_1] = eKeys_Pad_1;
            s_KeyCodex[GLFW_KEY_KP_2] = eKeys_Pad_2;
            s_KeyCodex[GLFW_KEY_KP_3] = eKeys_Pad_3;
            s_KeyCodex[GLFW_KEY_KP_4] = eKeys_Pad_4;
            s_KeyCodex[GLFW_KEY_KP_5] = eKeys_Pad_5;
            s_KeyCodex[GLFW_KEY_KP_6] = eKeys_Pad_6;
            s_KeyCodex[GLFW_KEY_KP_7] = eKeys_Pad_7;
            s_KeyCodex[GLFW_KEY_KP_8] = eKeys_Pad_8;
            s_KeyCodex[GLFW_KEY_KP_9] = eKeys_Pad_9;

            s_KeyCodex[GLFW_KEY_A] = eKeys_A;
            s_KeyCodex[GLFW_KEY_B] = eKeys_B;
            s_KeyCodex[GLFW_KEY_C] = eKeys_C;
            s_KeyCodex[GLFW_KEY_D] = eKeys_D;
            s_KeyCodex[GLFW_KEY_E] = eKeys_E;
            s_KeyCodex[GLFW_KEY_F] = eKeys_F;
            s_KeyCodex[GLFW_KEY_G] = eKeys_G;
            s_KeyCodex[GLFW_KEY_H] = eKeys_H;
            s_KeyCodex[GLFW_KEY_I] = eKeys_I;
            s_KeyCodex[GLFW_KEY_J] = eKeys_J;
            s_KeyCodex[GLFW_KEY_K] = eKeys_K;
            s_KeyCodex[GLFW_KEY_L] = eKeys_L;
            s_KeyCodex[GLFW_KEY_M] = eKeys_M;
            s_KeyCodex[GLFW_KEY_N] = eKeys_N;
            s_KeyCodex[GLFW_KEY_O] = eKeys_O;
            s_KeyCodex[GLFW_KEY_P] = eKeys_P;
            s_KeyCodex[GLFW_KEY_Q] = eKeys_Q;
            s_KeyCodex[GLFW_KEY_R] = eKeys_R;
            s_KeyCodex[GLFW_KEY_S] = eKeys_S;
            s_KeyCodex[GLFW_KEY_T] = eKeys_T;
            s_KeyCodex[GLFW_KEY_U] = eKeys_U;
            s_KeyCodex[GLFW_KEY_V] = eKeys_V;
            s_KeyCodex[GLFW_KEY_W] = eKeys_W;
            s_KeyCodex[GLFW_KEY_X] = eKeys_X;
            s_KeyCodex[GLFW_KEY_Y] = eKeys_Y;
            s_KeyCodex[GLFW_KEY_Z] = eKeys_Z;

            s_KeyCodex[GLFW_KEY_0] = eKeys_0;
            s_KeyCodex[GLFW_KEY_1] = eKeys_1;
            s_KeyCodex[GLFW_KEY_2] = eKeys_2;
            s_KeyCodex[GLFW_KEY_3] = eKeys_3;
            s_KeyCodex[GLFW_KEY_4] = eKeys_4;
            s_KeyCodex[GLFW_KEY_5] = eKeys_5;
            s_KeyCodex[GLFW_KEY_6] = eKeys_6;
            s_KeyCodex[GLFW_KEY_7] = eKeys_7;
            s_KeyCodex[GLFW_KEY_8] = eKeys_8;
            s_KeyCodex[GLFW_KEY_9] = eKeys_9;

            s_KeyCodex[GLFW_KEY_SPACE] = eKeys_Space;
            s_KeyCodex[GLFW_KEY_APOSTROPHE] = eKeys_Apostrophe;
            s_KeyCodex[GLFW_KEY_COMMA] = eKeys_Comma;
            s_KeyCodex[GLFW_KEY_PERIOD] = eKeys_Period;
            s_KeyCodex[GLFW_KEY_SLASH] = eKeys_ForwardSlash;
            s_KeyCodex[GLFW_KEY_BACKSLASH] = eKeys_BackSlash;
            s_KeyCodex[GLFW_KEY_SEMICOLON] = eKeys_SemiColon;
            s_KeyCodex[GLFW_KEY_LEFT_BRACKET] = eKeys_LeftBracket;
            s_KeyCodex[GLFW_KEY_RIGHT_BRACKET] = eKeys_RightBracket;
            s_KeyCodex[GLFW_KEY_GRAVE_ACCENT] = eKeys_Tilde;
            s_KeyCodex[GLFW_KEY_MINUS] = eKeys_Minus;
            s_KeyCodex[GLFW_KEY_EQUAL] = eKeys_Equal;

            s_KeyCodex[GLFW_KEY_UP] = eKeys_Up;
            s_KeyCodex[GLFW_KEY_DOWN] = eKeys_Down;
            s_KeyCodex[GLFW_KEY_LEFT] = eKeys_Left;
            s_KeyCodex[GLFW_KEY_RIGHT] = eKeys_Right;

            s_KeyCodex[GLFW_KEY_F1] = eKeys_F1;
            s_KeyCodex[GLFW_KEY_F2] = eKeys_F2;
            s_KeyCodex[GLFW_KEY_F3] = eKeys_F3;
            s_KeyCodex[GLFW_KEY_F4] = eKeys_F4;
            s_KeyCodex[GLFW_KEY_F5] = eKeys_F5;
            s_KeyCodex[GLFW_KEY_F6] = eKeys_F6;
            s_KeyCodex[GLFW_KEY_F7] = eKeys_F7;
            s_KeyCodex[GLFW_KEY_F8] = eKeys_F8;
            s_KeyCodex[GLFW_KEY_F9] = eKeys_F9;
            s_KeyCodex[GLFW_KEY_F10] = eKeys_F10;

            s_KeyCodex[GLFW_KEY_F11] = eKeys_F11;
            s_KeyCodex[GLFW_KEY_F12] = eKeys_F12;
            s_KeyCodex[GLFW_KEY_F13] = eKeys_F13;
            s_KeyCodex[GLFW_KEY_F14] = eKeys_F14;
            s_KeyCodex[GLFW_KEY_F15] = eKeys_F15;
            s_KeyCodex[GLFW_KEY_F16] = eKeys_F16;
            s_KeyCodex[GLFW_KEY_F17] = eKeys_F17;
            s_KeyCodex[GLFW_KEY_F18] = eKeys_F18;
            s_KeyCodex[GLFW_KEY_F19] = eKeys_F19;
            s_KeyCodex[GLFW_KEY_F20] = eKeys_F20;

            s_KeyCodex[GLFW_KEY_F21] = eKeys_F21;
            s_KeyCodex[GLFW_KEY_F22] = eKeys_F22;
            s_KeyCodex[GLFW_KEY_F23] = eKeys_F23;
            s_KeyCodex[GLFW_KEY_F24] = eKeys_F24;
            s_KeyCodex[GLFW_KEY_F25] = eKeys_F25;

            // System Keys
            // s_KeyCodex[eKeys_System] = -1; // Set to Max
        }

    }

}
