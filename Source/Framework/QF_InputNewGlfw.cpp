#ifdef _QGLFW3
#include "Libraries/glfw/glfw3.h"

// #include "QF_InputNew.h"

#include "QF_eKeys.h"
#include "QF_Window.h"

namespace QwerkE {

    namespace Input {

        extern void OnKeyEvent_New(eKeys a_Key, eKeyState a_KeyState);
        extern eKeys GLFWToQwerkEKey(int key); // #TODO Update or create a new mapping helper

        void Local_GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
        {
            const eKeys qwerkEeKey = GLFWToQwerkEKey(key);

            ASSERT(eKeys::eKeys_MAX != qwerkEeKey, "Unsupported key!");
            if (GLFW_REPEAT != action) // #TODO Support GLFW_REPEAT
            {
                OnKeyEvent_New(qwerkEeKey, (eKeyState)(action));
            }
        }

        // #TODO extern and call in Window::
        void Input_GlfwCallbacks(GLFWwindow* window)
        {
            glfwSetKeyCallback(window, Local_GlfwKeyCallback);
            // glfwSetCharCallback(window, char_callback);
            // glfwSetCharModsCallback(window, char_mods_callback);
            // glfwSetCursorPosCallback(window, cursor_position_callback);
            // glfwSetMouseButtonCallback(window, mouse_button_callback);
            // glfwSetJoystickCallback(joystick_callback);
            // glfwSetScrollCallback(window, scroll_callback);
        }

        int QwerkEKeyToGLFW(eKeys key) // #TODO Find a better key mapping solution
        {
            if (key < 0 || key >= GLFW_KEY_LAST)
            {
                LOG_ERROR("{0} Key code {1} out of range!", __FUNCTION__, key);
                return GLFW_KEY_LAST;
            }
            return key;
        }

        bool KeyDown(const eKeys a_Key)
        {
            // #TODO GLFWindow* reference. Remember to test multi-window input
            return GLFW_PRESS == glfwGetKey(static_cast<GLFWwindow*>(Window::GetContext()), QwerkEKeyToGLFW(a_Key));
        }

        constexpr int QwerkEToGlfw(const eKeys a_QwerkEKey)
        {
            // #TODO Implement
            switch (a_QwerkEKey)
            {
            default:
                break;
            }

            ASSERT(false, "Unsupported QwerkE key!");
            return GLFW_KEY_LAST;
        }

        constexpr eKeys GlfwToQwerkE(const int a_GlfwKey)
        {
            switch (a_GlfwKey) // GLFW keys defined in glfw3.h, or https://www.glfw.org/docs/latest/group__keys.html
            {
                case GLFW_KEY_SPACE:            return eKeys::eKeys_Space;          // 32 ' '

                case GLFW_KEY_APOSTROPHE:       return eKeys::eKeys_Apostrophe;     // 39 '''

                case GLFW_KEY_COMMA:            return eKeys::eKeys_Comma;          // 44 ','
                case GLFW_KEY_MINUS:            return eKeys::eKeys_Minus;          // 45 '-'
                case GLFW_KEY_PERIOD:           return eKeys::eKeys_Period;         // 46 '.'
                case GLFW_KEY_SLASH:            return eKeys::eKeys_ForwardSlash;   // 47 '/'
                case GLFW_KEY_0:                return eKeys::eKeys_0;              // 48
                case GLFW_KEY_1:                return eKeys::eKeys_1;              // 49
                case GLFW_KEY_2:                return eKeys::eKeys_2;              // 50
                case GLFW_KEY_3:                return eKeys::eKeys_3;              // 51
                case GLFW_KEY_4:                return eKeys::eKeys_4;              // 52
                case GLFW_KEY_5:                return eKeys::eKeys_5;              // 53
                case GLFW_KEY_6:                return eKeys::eKeys_6;              // 54
                case GLFW_KEY_7:                return eKeys::eKeys_7;              // 55
                case GLFW_KEY_8:                return eKeys::eKeys_8;              // 56
                case GLFW_KEY_9:                return eKeys::eKeys_9;              // 57

                case GLFW_KEY_SEMICOLON:        return eKeys::eKeys_SemiColon;      // 59 ';'

                case GLFW_KEY_EQUAL:            return eKeys::eKeys_Equal;          // 61 '='

                case GLFW_KEY_A:                return eKeys::eKeys_A;              // 65
                case GLFW_KEY_B:                return eKeys::eKeys_B;              // 66
                case GLFW_KEY_C:                return eKeys::eKeys_C;              // 67
                case GLFW_KEY_D:                return eKeys::eKeys_D;              // 68
                case GLFW_KEY_E:                return eKeys::eKeys_E;              // 69
                case GLFW_KEY_F:                return eKeys::eKeys_F;              // 70
                case GLFW_KEY_G:                return eKeys::eKeys_G;              // 71
                case GLFW_KEY_H:                return eKeys::eKeys_H;              // 72
                case GLFW_KEY_I:                return eKeys::eKeys_I;              // 73
                case GLFW_KEY_J:                return eKeys::eKeys_J;              // 74
                case GLFW_KEY_K:                return eKeys::eKeys_K;              // 75
                case GLFW_KEY_L:                return eKeys::eKeys_L;              // 76
                case GLFW_KEY_M:                return eKeys::eKeys_M;              // 77
                case GLFW_KEY_N:                return eKeys::eKeys_N;              // 78
                case GLFW_KEY_O:                return eKeys::eKeys_O;              // 79
                case GLFW_KEY_P:                return eKeys::eKeys_P;              // 80
                case GLFW_KEY_Q:                return eKeys::eKeys_Q;              // 81
                case GLFW_KEY_R:                return eKeys::eKeys_R;              // 82
                case GLFW_KEY_S:                return eKeys::eKeys_S;              // 83
                case GLFW_KEY_T:                return eKeys::eKeys_T;              // 84
                case GLFW_KEY_U:                return eKeys::eKeys_U;              // 85
                case GLFW_KEY_V:                return eKeys::eKeys_V;              // 86
                case GLFW_KEY_W:                return eKeys::eKeys_W;              // 87
                case GLFW_KEY_X:                return eKeys::eKeys_X;              // 88
                case GLFW_KEY_Y:                return eKeys::eKeys_Y;              // 89
                case GLFW_KEY_Z:                return eKeys::eKeys_Z;              // 90
                case GLFW_KEY_LEFT_BRACKET:     return eKeys::eKeys_LeftBracket;    // 91 '['
                case GLFW_KEY_BACKSLASH:        return eKeys::eKeys_BackSlash;      // 92 '\'
                case GLFW_KEY_RIGHT_BRACKET:    return eKeys::eKeys_RightBracket;   // 93 ']'

                case GLFW_KEY_GRAVE_ACCENT:     return eKeys::eKeys_Tilde;          // 96 '`'

                // #TODO Need eKeys entry case GLFW_KEY_WORLD_1: return eKeys::eKeys_?; // 161 non-US #1
                // #TODO Need eKeys entry case GLFW_KEY_WORLD_2: return eKeys::eKeys_?; // 162 non-US #2

                case GLFW_KEY_ESCAPE:           return eKeys::eKeys_Escape;         // 256
                case GLFW_KEY_ENTER:            return eKeys::eKeys_Return;         // 257
                case GLFW_KEY_TAB:              return eKeys::eKeys_Tab;            // 258
                case GLFW_KEY_BACKSPACE:        return eKeys::eKeys_Backspace;      // 259
                case GLFW_KEY_INSERT:           return eKeys::eKeys_Insert;         // 260
                case GLFW_KEY_DELETE:           return eKeys::eKeys_Delete;         // 261
                case GLFW_KEY_RIGHT:            return eKeys::eKeys_Right;          // 262
                case GLFW_KEY_LEFT:             return eKeys::eKeys_Left;           // 263
                case GLFW_KEY_DOWN:             return eKeys::eKeys_Down;           // 264
                case GLFW_KEY_UP:               return eKeys::eKeys_Up;             // 265
                case GLFW_KEY_PAGE_UP:          return eKeys::eKeys_PageUp;         // 266
                case GLFW_KEY_PAGE_DOWN:        return eKeys::eKeys_PageDown;       // 267
                case GLFW_KEY_HOME:             return eKeys::eKeys_Home;           // 268
                case GLFW_KEY_END:              return eKeys::eKeys_End;            // 269

                case GLFW_KEY_CAPS_LOCK:        return eKeys::eKeys_CapsLock;       // 280
                case GLFW_KEY_SCROLL_LOCK:      return eKeys::eKeys_ScrollLock;     // 281
                case GLFW_KEY_NUM_LOCK:         return eKeys::eKeys_NumLock;        // 282
                case GLFW_KEY_PRINT_SCREEN:     return eKeys::eKeys_PrintScreen;    // 283
                case GLFW_KEY_PAUSE:            return eKeys::eKeys_Pause;          // 284

                case GLFW_KEY_F1:               return eKeys::eKeys_F1;             // 290
                case GLFW_KEY_F2:               return eKeys::eKeys_F2;             // 291
                case GLFW_KEY_F3:               return eKeys::eKeys_F3;             // 292
                case GLFW_KEY_F4:               return eKeys::eKeys_F4;             // 293
                case GLFW_KEY_F5:               return eKeys::eKeys_F5;             // 294
                case GLFW_KEY_F6:               return eKeys::eKeys_F6;             // 295
                case GLFW_KEY_F7:               return eKeys::eKeys_F7;             // 296
                case GLFW_KEY_F8:               return eKeys::eKeys_F8;             // 297
                case GLFW_KEY_F9:               return eKeys::eKeys_F9;             // 298
                case GLFW_KEY_F10:              return eKeys::eKeys_F10;            // 299
                case GLFW_KEY_F11:              return eKeys::eKeys_F11;            // 300
                case GLFW_KEY_F12:              return eKeys::eKeys_F12;            // 301
                case GLFW_KEY_F13:              return eKeys::eKeys_F13;            // 302
                case GLFW_KEY_F14:              return eKeys::eKeys_F14;            // 303
                case GLFW_KEY_F15:              return eKeys::eKeys_F15;            // 304
                case GLFW_KEY_F16:              return eKeys::eKeys_F16;            // 305
                case GLFW_KEY_F17:              return eKeys::eKeys_F17;            // 306
                case GLFW_KEY_F18:              return eKeys::eKeys_F18;            // 307
                case GLFW_KEY_F19:              return eKeys::eKeys_F19;            // 308
                case GLFW_KEY_F20:              return eKeys::eKeys_F20;            // 309
                case GLFW_KEY_F21:              return eKeys::eKeys_F21;            // 310
                case GLFW_KEY_F22:              return eKeys::eKeys_F22;            // 311
                case GLFW_KEY_F23:              return eKeys::eKeys_F23;            // 312
                case GLFW_KEY_F24:              return eKeys::eKeys_F24;            // 313
                case GLFW_KEY_F25:              return eKeys::eKeys_F25;            // 314

                case GLFW_KEY_KP_0:             return eKeys::eKeys_Pad_0;          // 320
                case GLFW_KEY_KP_1:             return eKeys::eKeys_Pad_1;          // 321
                case GLFW_KEY_KP_2:             return eKeys::eKeys_Pad_2;          // 322
                case GLFW_KEY_KP_3:             return eKeys::eKeys_Pad_3;          // 323
                case GLFW_KEY_KP_4:             return eKeys::eKeys_Pad_4;          // 324
                case GLFW_KEY_KP_5:             return eKeys::eKeys_Pad_5;          // 325
                case GLFW_KEY_KP_6:             return eKeys::eKeys_Pad_6;          // 326
                case GLFW_KEY_KP_7:             return eKeys::eKeys_Pad_7;          // 327
                case GLFW_KEY_KP_8:             return eKeys::eKeys_Pad_8;          // 328
                case GLFW_KEY_KP_9:             return eKeys::eKeys_Pad_9;          // 329

                case GLFW_KEY_KP_DECIMAL:       return eKeys::eKeys_Pad_Decimal;    // 330
                case GLFW_KEY_KP_DIVIDE:        return eKeys::eKeys_Pad_Division;   // 331
                case GLFW_KEY_KP_MULTIPLY:      return eKeys::eKeys_Pad_Multiply;   // 332
                case GLFW_KEY_KP_SUBTRACT:      return eKeys::eKeys_Pad_Minus;      // 333
                case GLFW_KEY_KP_ADD:           return eKeys::eKeys_Pad_Plus;       // 334
                case GLFW_KEY_KP_ENTER:         return eKeys::eKeys_Pad_Return;     // 335
                case GLFW_KEY_KP_EQUAL:         return eKeys::eKeys_Pad_Equal;      // 336

                case GLFW_KEY_LEFT_SHIFT:       return eKeys::eKeys_LSHIFT;         // 340
                case GLFW_KEY_LEFT_CONTROL:     return eKeys::eKeys_LCTRL;          // 341
                case GLFW_KEY_LEFT_ALT:         return eKeys::eKeys_LALT;           // 342
                case GLFW_KEY_LEFT_SUPER:       return eKeys::eKeys_LSUPER;         // 343
                case GLFW_KEY_RIGHT_SHIFT:      return eKeys::eKeys_RSHIFT;         // 344
                case GLFW_KEY_RIGHT_CONTROL:    return eKeys::eKeys_RCTRL;          // 345
                case GLFW_KEY_RIGHT_ALT:        return eKeys::eKeys_RALT;           // 346
                case GLFW_KEY_RIGHT_SUPER:      return eKeys::eKeys_RSUPER;         // 347
                case GLFW_KEY_MENU:             return eKeys::eKeys_Menu;           // 348

                default: // GLFW_KEY_LAST = GLFW_KEY_MENU
                    break;
            }

            ASSERT(false, "Unsupported GLFW key!");
            return eKeys::eKeys_MAX;
        }

    }

}
#endif
