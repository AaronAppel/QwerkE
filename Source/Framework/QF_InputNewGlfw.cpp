#ifdef _QGLFW3
#include "Libraries/glfw/glfw3.h"

#include "QF_QKey.h"
#include "QF_Window.h"

namespace QwerkE {

    namespace Input {

        extern void OnKeyEvent_New(const QKey a_Key, const bool a_KeyState);
        extern u8 s_KeysCurrentlyDown;

        constexpr QKey GlfwToQwerkE(const int a_GlfwKey, int a_Scancode);

        void Local_GlfwKeyCallback(GLFWwindow* a_Window, int a_Key, int a_Scancode, int a_Action, int a_Mode)
        {
            // #TODO Check values on media key presses. What is scan code?
            if (GLFW_REPEAT != a_Action) // #TODO Review GLFW_REPEAT
            {
                const QKey qwerkEeKey = GlfwToQwerkE(a_Key, a_Scancode);
                if (QKey::e_MAX != qwerkEeKey)
                {
                    OnKeyEvent_New(qwerkEeKey, GLFW_PRESS == a_Action);
                }
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

        bool KeyDown(const QKey a_Key)
        {
            if (e_Any == a_Key)
            {
                return s_KeysCurrentlyDown > 0;
            }
            // #TODO Support e_Any
            // #TODO GLFWindow* reference. Remember to test multi-window input
            const int scancode = 0; // #TODO Look at switching to scan codes
            return GLFW_PRESS == glfwGetKey(static_cast<GLFWwindow*>(Window::GetContext()), GlfwToQwerkE(a_Key, scancode));
        }

        constexpr int QwerkEToGlfw(const QKey a_QwerkEKey)
        {
            // #TODO Implement
            switch (a_QwerkEKey)
            {
            case -1: return -1;
            case QKey::e_MAX: return GLFW_KEY_LAST;

            default:
                break;
            }

            ASSERT(false, "Unsupported QwerkE key!");
            return GLFW_KEY_LAST;
        }

        constexpr QKey GlfwToQwerkE(const int a_GlfwKey, int a_Scancode)
        {
            switch (a_GlfwKey) // GLFW keys defined in glfw3.h, or https://www.glfw.org/docs/latest/group__keys.html
            {
                case GLFW_KEY_SPACE:            return QKey::e_Space;          // 32 ' '

                case GLFW_KEY_APOSTROPHE:       return QKey::e_Apostrophe;     // 39 '''

                case GLFW_KEY_COMMA:            return QKey::e_Comma;           // 44 ','
                case GLFW_KEY_MINUS:            return QKey::e_Minus;           // 45 '-'
                case GLFW_KEY_PERIOD:           return QKey::e_Period;          // 46 '.'
                case GLFW_KEY_SLASH:            return QKey::e_SlashForward;    // 47 '/'
                case GLFW_KEY_0:                return QKey::e_0;               // 48
                case GLFW_KEY_1:                return QKey::e_1;               // 49
                case GLFW_KEY_2:                return QKey::e_2;               // 50
                case GLFW_KEY_3:                return QKey::e_3;               // 51
                case GLFW_KEY_4:                return QKey::e_4;               // 52
                case GLFW_KEY_5:                return QKey::e_5;               // 53
                case GLFW_KEY_6:                return QKey::e_6;               // 54
                case GLFW_KEY_7:                return QKey::e_7;               // 55
                case GLFW_KEY_8:                return QKey::e_8;               // 56
                case GLFW_KEY_9:                return QKey::e_9;               // 57

                case GLFW_KEY_SEMICOLON:        return QKey::e_SemiColon;       // 59 ';'

                case GLFW_KEY_EQUAL:            return QKey::e_Equal;           // 61 '='

                case GLFW_KEY_A:                return QKey::e_A;               // 65
                case GLFW_KEY_B:                return QKey::e_B;               // 66
                case GLFW_KEY_C:                return QKey::e_C;               // 67
                case GLFW_KEY_D:                return QKey::e_D;               // 68
                case GLFW_KEY_E:                return QKey::e_E;               // 69
                case GLFW_KEY_F:                return QKey::e_F;               // 70
                case GLFW_KEY_G:                return QKey::e_G;               // 71
                case GLFW_KEY_H:                return QKey::e_H;               // 72
                case GLFW_KEY_I:                return QKey::e_I;               // 73
                case GLFW_KEY_J:                return QKey::e_J;               // 74
                case GLFW_KEY_K:                return QKey::e_K;               // 75
                case GLFW_KEY_L:                return QKey::e_L;               // 76
                case GLFW_KEY_M:                return QKey::e_M;               // 77
                case GLFW_KEY_N:                return QKey::e_N;               // 78
                case GLFW_KEY_O:                return QKey::e_O;               // 79
                case GLFW_KEY_P:                return QKey::e_P;               // 80
                case GLFW_KEY_Q:                return QKey::e_Q;               // 81
                case GLFW_KEY_R:                return QKey::e_R;               // 82
                case GLFW_KEY_S:                return QKey::e_S;               // 83
                case GLFW_KEY_T:                return QKey::e_T;               // 84
                case GLFW_KEY_U:                return QKey::e_U;               // 85
                case GLFW_KEY_V:                return QKey::e_V;               // 86
                case GLFW_KEY_W:                return QKey::e_W;               // 87
                case GLFW_KEY_X:                return QKey::e_X;               // 88
                case GLFW_KEY_Y:                return QKey::e_Y;               // 89
                case GLFW_KEY_Z:                return QKey::e_Z;               // 90
                case GLFW_KEY_LEFT_BRACKET:     return QKey::e_BracketLeft;     // 91 '['
                case GLFW_KEY_BACKSLASH:        return QKey::e_BackSlash;       // 92 '\'
                case GLFW_KEY_RIGHT_BRACKET:    return QKey::e_BracketRight;    // 93 ']'

                case GLFW_KEY_GRAVE_ACCENT:     return QKey::e_Tilde;           // 96 '`'

                // #TODO Need QKey entry case GLFW_KEY_WORLD_1: return QKey::e_?; // 161 non-US #1
                // #TODO Need QKey entry case GLFW_KEY_WORLD_2: return QKey::e_?; // 162 non-US #2

                case GLFW_KEY_ESCAPE:           return QKey::e_Escape;          // 256
                case GLFW_KEY_ENTER:            return QKey::e_Enter;           // 257
                case GLFW_KEY_TAB:              return QKey::e_Tab;             // 258
                case GLFW_KEY_BACKSPACE:        return QKey::e_Backspace;       // 259
                case GLFW_KEY_INSERT:           return QKey::e_Insert;          // 260
                case GLFW_KEY_DELETE:           return QKey::e_Delete;          // 261
                case GLFW_KEY_RIGHT:            return QKey::e_Right;           // 262
                case GLFW_KEY_LEFT:             return QKey::e_Left;            // 263
                case GLFW_KEY_DOWN:             return QKey::e_Down;            // 264
                case GLFW_KEY_UP:               return QKey::e_Up;              // 265
                case GLFW_KEY_PAGE_UP:          return QKey::e_PageUp;          // 266
                case GLFW_KEY_PAGE_DOWN:        return QKey::e_PageDown;        // 267
                case GLFW_KEY_HOME:             return QKey::e_Home;            // 268
                case GLFW_KEY_END:              return QKey::e_End;             // 269

                case GLFW_KEY_CAPS_LOCK:        return QKey::e_CapsLock;        // 280
                case GLFW_KEY_SCROLL_LOCK:      return QKey::e_ScrollLock;      // 281
                case GLFW_KEY_NUM_LOCK:         return QKey::e_NumLock;         // 282
                case GLFW_KEY_PRINT_SCREEN:     return QKey::e_PrintScreen;     // 283
                case GLFW_KEY_PAUSE:            return QKey::e_Pause;           // 284

                case GLFW_KEY_F1:               return QKey::e_F1;              // 290
                case GLFW_KEY_F2:               return QKey::e_F2;              // 291
                case GLFW_KEY_F3:               return QKey::e_F3;              // 292
                case GLFW_KEY_F4:               return QKey::e_F4;              // 293
                case GLFW_KEY_F5:               return QKey::e_F5;              // 294
                case GLFW_KEY_F6:               return QKey::e_F6;              // 295
                case GLFW_KEY_F7:               return QKey::e_F7;              // 296
                case GLFW_KEY_F8:               return QKey::e_F8;              // 297
                case GLFW_KEY_F9:               return QKey::e_F9;              // 298
                case GLFW_KEY_F10:              return QKey::e_F10;             // 299
                case GLFW_KEY_F11:              return QKey::e_F11;             // 300
                case GLFW_KEY_F12:              return QKey::e_F12;             // 301
                case GLFW_KEY_F13:              return QKey::e_F13;             // 302
                case GLFW_KEY_F14:              return QKey::e_F14;             // 303
                case GLFW_KEY_F15:              return QKey::e_F15;             // 304
                case GLFW_KEY_F16:              return QKey::e_F16;             // 305
                case GLFW_KEY_F17:              return QKey::e_F17;             // 306
                case GLFW_KEY_F18:              return QKey::e_F18;             // 307
                case GLFW_KEY_F19:              return QKey::e_F19;             // 308
                case GLFW_KEY_F20:              return QKey::e_F20;             // 309
                case GLFW_KEY_F21:              return QKey::e_F21;             // 310
                case GLFW_KEY_F22:              return QKey::e_F22;             // 311
                case GLFW_KEY_F23:              return QKey::e_F23;             // 312
                case GLFW_KEY_F24:              return QKey::e_F24;             // 313
                case GLFW_KEY_F25:              return QKey::e_F25;             // 314

                case GLFW_KEY_KP_0:             return QKey::e_Pad_0;           // 320
                case GLFW_KEY_KP_1:             return QKey::e_Pad_1;           // 321
                case GLFW_KEY_KP_2:             return QKey::e_Pad_2;           // 322
                case GLFW_KEY_KP_3:             return QKey::e_Pad_3;           // 323
                case GLFW_KEY_KP_4:             return QKey::e_Pad_4;           // 324
                case GLFW_KEY_KP_5:             return QKey::e_Pad_5;           // 325
                case GLFW_KEY_KP_6:             return QKey::e_Pad_6;           // 326
                case GLFW_KEY_KP_7:             return QKey::e_Pad_7;           // 327
                case GLFW_KEY_KP_8:             return QKey::e_Pad_8;           // 328
                case GLFW_KEY_KP_9:             return QKey::e_Pad_9;           // 329

                case GLFW_KEY_KP_DECIMAL:       return QKey::e_Pad_Decimal;     // 330
                case GLFW_KEY_KP_DIVIDE:        return QKey::e_Pad_Division;    // 331
                case GLFW_KEY_KP_MULTIPLY:      return QKey::e_Pad_Multiply;    // 332
                case GLFW_KEY_KP_SUBTRACT:      return QKey::e_Pad_Minus;       // 333
                case GLFW_KEY_KP_ADD:           return QKey::e_Pad_Plus;        // 334
                case GLFW_KEY_KP_ENTER:         return QKey::e_Pad_Return;      // 335
                case GLFW_KEY_KP_EQUAL:         return QKey::e_Pad_Equal;       // 336

                case GLFW_KEY_LEFT_SHIFT:       return QKey::e_ShiftL;          // 340
                case GLFW_KEY_LEFT_CONTROL:     return QKey::e_CtrlL;           // 341
                case GLFW_KEY_LEFT_ALT:         return QKey::e_AltL;            // 342
                case GLFW_KEY_LEFT_SUPER:       return QKey::e_SystemL;         // 343
                case GLFW_KEY_RIGHT_SHIFT:      return QKey::e_ShiftR;          // 344
                case GLFW_KEY_RIGHT_CONTROL:    return QKey::e_CtrlR;           // 345
                case GLFW_KEY_RIGHT_ALT:        return QKey::e_AltR;            // 346
                case GLFW_KEY_RIGHT_SUPER:      return QKey::e_SystemR;         // 347
                case GLFW_KEY_MENU:             return QKey::e_Menu;            // 348
                // GLFW_KEY_LAST == GLFW_KEY_MENU == 348
            }

            switch (a_Scancode)
            {
            case 290: return QKey::e_MediaPlay;
            case 302: return QKey::e_MediaVolumeDown;
            case 304: return QKey::e_MediaVolumeUp;
            // case 302: return QKey::e_MediaVolumeMute;

            default:
                break;
            }

            // Media play/pause, media volume up and down keys are not supported by GLFW
            LOG_CRITICAL("Unsupported GLFW key {0}!", a_GlfwKey);
            // ASSERT(false, "Unsupported GLFW key!");
            return QKey::e_MAX;
        }

    }

}
#endif
