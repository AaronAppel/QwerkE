#ifdef _QGLFW3
#include <vector>

#include "Libraries/glfw/glfw3.h"
#include "Libraries/imgui/imgui.h"

#include "QF_QKey.h"
#include "QF_InputStatesBitRingBuffer.h"
#include "QF_Window.h"

namespace QwerkE {

    namespace Input {

        // #TODO Consider using extern to access GLFW window* in Window::
        // extern GLFWwindow* s_GlfwWindow;

        extern InputStatesBitRingBuffer<QKey, u5> s_Keys;
        extern InputStatesBitRingBuffer<QKey, u3> s_MouseButtons;
        extern std::vector<std::pair<QGamepad, InputStatesBitRingBuffer<QGamepad, u4>>> s_GamepadsButtons;

        extern void Internal_Initialize();
        extern void Internal_NewFrame();
        extern void Internal_Shutdown();

        extern void Internal_KeyChanged(const QKey a_Key, const QKeyState a_KeyState);

        extern void Internal_MouseMove(const vec2f& a_NewPosition);
        extern void Internal_MouseButton(const QKey a_Key, const QKeyState a_KeyState);
        extern void Internal_MouseScroll(const double xoffset, const double yoffset);

        extern void Internal_GamepadAxis(const unsigned char a_AxisId, const vec2f a_AxisValue, const QGamepad a_GamepadId);
        extern void Internal_GamepadButton(const QGamepad a_Key, const QKeyState a_KeyState, const QGamepad a_GamepadId);

        std::vector<int> s_GamepadIds; // #TODO Review glfwUpdateGamepadMappings, glfwGetGamepadName, glfwGetGamepadState
        static std::vector<std::vector<float>> s_DeviceAxesStates;
        static std::vector<std::vector<int>> s_DeviceButtonsStates;

        static void Local_AddGamepad(const u8 a_GamepadId);
        static constexpr int Local_QwerkEToGlfw(const QKey a_QwerkEKey);
        static constexpr QKey Local_GlfwToQwerkE(const int a_GlfwKey, int a_Scancode);

        void Initialize()
        {
            Internal_Initialize();

            for (size_t i = 0; i < GLFW_JOYSTICK_LAST; i++)
            {
                int present = glfwJoystickPresent(i);
                if (present)
                {
                    Local_AddGamepad(i);
                }
            }
        }

        void NewFrame()
        {
            Internal_NewFrame();

            glfwJoystickPresent(QGamepad::e_Gamepad0); // #NOTE Triggers GLFW to poll joystick disconnect event(s)

            for (size_t i = 0; i < s_GamepadIds.size(); i++)
            {
                // #TODO Review glfwGetGamepadState https://www.glfw.org/docs/latest/group__input.html#gadccddea8bce6113fa459de379ddaf051

                ASSERT(glfwJoystickPresent(s_GamepadIds[i]), "Device no longer present!");

                int axesCount;
                const float* axes = glfwGetJoystickAxes(s_GamepadIds[i], &axesCount);
                ASSERT(axes, "Null device axes!");

                vec2f leftStickAxes;
                vec2f rightStickAxes;
                vec2f triggersAxes;

                for (size_t j = 0; j < axesCount; j++)
                {
                    switch (j)
                    {
                    case 0: // Left stick X [0]
                        leftStickAxes.x = axes[j];
                        break;
                    case 1: // Left stick Y [1]
                        leftStickAxes.y = axes[j];
                        if (s_DeviceAxesStates[i][0] != leftStickAxes.x || s_DeviceAxesStates[i][1] != leftStickAxes.y)
                        {
                            Internal_GamepadAxis(j, leftStickAxes, static_cast<QGamepad>(s_GamepadIds[i]));
                        }
                        break;
                    case 2: // Right stick X [2]
                        rightStickAxes.x = axes[j];
                        break;
                    case 3: // Right stick Y [3]
                        rightStickAxes.y = axes[j];
                        if (s_DeviceAxesStates[i][2] != rightStickAxes.x || s_DeviceAxesStates[i][3] != rightStickAxes.y)
                        {
                            Internal_GamepadAxis(j, rightStickAxes, static_cast<QGamepad>(s_GamepadIds[i]));
                        }
                        break;
                    case 4: // L trigger [4]
                        triggersAxes.x = axes[j];
                        break;
                    case 5: // R trigger [5]
                        triggersAxes.y = axes[j];
                        if (s_DeviceAxesStates[i][4] != triggersAxes.x || s_DeviceAxesStates[i][5] != triggersAxes.y)
                        {
                            Internal_GamepadAxis(j, triggersAxes, static_cast<QGamepad>(s_GamepadIds[i]));
                        }
                        break;
                    }
                }

                s_DeviceAxesStates[i][0] = leftStickAxes.x;
                s_DeviceAxesStates[i][1] = leftStickAxes.y;

                s_DeviceAxesStates[i][2] = rightStickAxes.x;
                s_DeviceAxesStates[i][3] = rightStickAxes.y;

                s_DeviceAxesStates[i][4] = triggersAxes.x;
                s_DeviceAxesStates[i][5] = triggersAxes.y;

                // #TODO Checkout glfwGetJoystickHats

                int hatStatesCount;
                const unsigned char* const buttons = glfwGetJoystickButtons(s_GamepadIds[i], &hatStatesCount);
                ASSERT(buttons, "Null device buttons!");

                for (size_t j = 0; j < hatStatesCount; j++)
                {
                    if (s_DeviceButtonsStates[i][j] != buttons[j])
                    {
                        Internal_GamepadButton(static_cast<QGamepad>(QGamepad::e_Gamepad0 + j), static_cast<QKeyState>(QKeyState::e_KeyStateDown == buttons[j]), static_cast<QGamepad>(s_GamepadIds[i]));
                    }
                    s_DeviceButtonsStates[i][j] = buttons[j];
                }
            }
        }

        void Shutdown()
        {
            Internal_Shutdown();

            s_GamepadIds.clear();
            s_DeviceButtonsStates.clear();
            s_DeviceAxesStates.clear();
        }

        bool KeyDown(const QKey a_Key)
        {
            if (e_Any == a_Key)
            {
                return s_Keys.DownKeys() > 0;
            }
            // #TODO GLFWindow* reference. Remember to test multi-window input
            return GLFW_PRESS == glfwGetKey(static_cast<GLFWwindow*>(Window::GetContext()), Local_QwerkEToGlfw(a_Key));
        }

        bool MouseDown(const QKey a_Key)
        {
            if (e_Any == a_Key)
            {
                return s_MouseButtons.DownKeys() > 0;
            }
            // #TODO GLFWindow* reference. Remember to test multi-window input
            // #TODO Handle invalid key. Maybe move mouse and joystick buttons into their own enum
            return GLFW_PRESS == glfwGetMouseButton(static_cast<GLFWwindow*>(Window::GetContext()), Local_QwerkEToGlfw(a_Key));
        }

        bool GamepadDown(const QGamepad a_Key, const QGamepad a_GamepadId)
        {
            ASSERT(QGamepad::e_GamepadAny == a_Key || QGamepad::e_QGamepadInputMax >= a_Key && QGamepad::e_Gamepad0 <= a_Key, "Invalid Gamepad key!");
            ASSERT(QGamepad::e_GamepadId0 <= a_GamepadId && QGamepad::e_QGamepadIdMax > a_GamepadId, "Invalid a_GamepadId!");
            if (s_DeviceButtonsStates.empty())
            {
                return false;
            }

            if (e_GamepadAny == a_Key)
            {
                for (u8 i = 0; i < s_GamepadsButtons.size(); i++)
                {
                    if (a_GamepadId == s_GamepadsButtons[i].first)
                    {
                        return s_GamepadsButtons[i].second.DownKeys() > 0;
                    }
                }
            }
            // #TODO GLFWindow* reference. Remember to test multi-window input
            s_DeviceButtonsStates;

            if (QGamepad::e_GamepadAny == a_Key)
            {
                for (u8 i = 0; i < s_DeviceButtonsStates.size(); i++)
                {
                    if (e_KeyStateDown == s_DeviceButtonsStates[a_GamepadId][i])
                    {
                        return true;
                    }
                }
                return false;
            }

            return e_KeyStateDown == s_DeviceButtonsStates[a_GamepadId][a_Key];
        }

        const char* const GamepadName(const QGamepad a_Key, const QGamepad a_GamepadId)
        {
            if (QGamepad::e_GamepadId0 <= a_Key && QGamepad::e_QGamepadIdMax > a_Key)
            {
                return glfwGetJoystickName(static_cast<int>(a_Key));
            }
            return nullptr;
        }

        u8 GamepadsCount()
        {
            return s_GamepadIds.size();
        }

        u8 GamepadButtonCount(const QGamepad a_Key, const QGamepad a_GamepadId)
        {
            if (QGamepad::e_GamepadId0 <= a_Key && QGamepad::e_QGamepadIdMax > a_Key)
            {
                int buttonCount;
                const unsigned char* const buttons = glfwGetJoystickButtons(static_cast<int>(a_Key), &buttonCount);
                return buttonCount;
            }
            return 0;
        }

        u8 GamepadAxesCount(const QGamepad a_Key, const QGamepad a_GamepadId)
        {
            if (QGamepad::e_GamepadId0 <= a_Key && QGamepad::e_QGamepadIdMax > a_Key)
            {
                int axesCount;
                const float* axes = glfwGetJoystickAxes(static_cast<int>(a_Key), &axesCount);
                return axesCount;
            }
            return 0;
        }

        static void Local_AddGamepad(const u8 a_GamepadId)
        {
            for (size_t i = 0; i < s_GamepadIds.size(); i++)
            {
                if (a_GamepadId == s_GamepadIds[i])
                {
                    ASSERT(false, "Device already registered!");
                    return;
                }
            }

            s_GamepadIds.emplace_back(a_GamepadId);
            std::pair<QGamepad, InputStatesBitRingBuffer<QGamepad, u4>> var = { (QGamepad)a_GamepadId , {} };
            s_GamepadsButtons.emplace_back(var); // #TODO Review safety

            int hatStatesCount;
            const unsigned char* const buttons = glfwGetJoystickButtons(a_GamepadId, &hatStatesCount);
            s_DeviceButtonsStates.emplace_back(0);
            for (size_t i = 0; i < hatStatesCount; i++)
            {
                s_DeviceButtonsStates[s_DeviceButtonsStates.size() - 1].emplace_back(buttons[i]);
            }

            int axesCount;
            const float* axes = glfwGetJoystickAxes(a_GamepadId, &axesCount);
            s_DeviceAxesStates.emplace_back(0.f);
            for (size_t i = 0; i < axesCount; i++)
            {
                s_DeviceAxesStates[s_DeviceAxesStates.size() - 1].emplace_back(axes[i]);
            }

            // #TODO Look at saving more gamepad data
            // glfwGetJoystickName
            // glfwGetJoystickGUID
            // glfwJoystickIsGamepad
        }

#ifdef _QDEARIMGUI
        // #NOTE Implementing using demo from: https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_glfw.cpp
        // #TODO See ImGui_ImplGlfw_UpdateMouseCursor() in https://github.com/ocornut/imgui/blob/docking/backends/imgui_impl_glfw.cpp#L851
        // Also see ImGui_ImplGlfw_UpdateMouseData() in https://github.com/ocornut/imgui/blob/docking/backends/imgui_impl_glfw.cpp#L780
        // Also see ImGui_ImplGlfw_UpdateGamepads() in https://github.com/ocornut/imgui/blob/docking/backends/imgui_impl_glfw.cpp#L880
        static void ImGui_ImplGlfw_UpdateKeyModifiers(GLFWwindow* window);
        static ImGuiKey ImGui_ImplGlfw_KeyToImGuiKey(int keycode, int scancode);
#endif // _QDEARIMGUI

        static void Local_GlfwKeyCallback(GLFWwindow* a_Window, const int a_KeyCode, const int a_Scancode, const int a_Action, const int a_Mode)
        {
            if (GLFW_REPEAT != a_Action) // #TODO Review GLFW_REPEAT
            {
                const QKey qwerkEeKey = Local_GlfwToQwerkE(a_KeyCode, a_Scancode);
                if (QKey::e_QKeyMax != qwerkEeKey)
                {
                    Internal_KeyChanged(qwerkEeKey, static_cast<QKeyState>(GLFW_PRESS == a_Action));
                }

#ifdef _QDEARIMGUI
                ImGui_ImplGlfw_UpdateKeyModifiers(a_Window);
                // #NOTE ImGui_ImplGlfw_TranslateUntranslatedKey() contents were stripped by #ifs so removed entirely

                ImGuiIO& io = ImGui::GetIO();
                ImGuiKey imgui_key = ImGui_ImplGlfw_KeyToImGuiKey(a_KeyCode, a_Scancode);
                io.AddKeyEvent(imgui_key, (a_Action == GLFW_PRESS));
#endif // _QDEARIMGUI
            }
        }

        static void Local_GlfwCharCallback(GLFWwindow* window, unsigned int codePoint)
        {
#ifdef _QDEARIMGUI
            if (codePoint > 0 && codePoint < 0x10000)
            {
                ImGui::GetIO().AddInputCharacter(codePoint);
            }
#endif // _QDEARIMGUI
        }

        static void Local_GlfwCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
        {
            Internal_MouseMove({ xpos, ypos });

#ifdef _QDEARIMGUI
            ImGuiIO& io = ImGui::GetIO();
            io.AddMousePosEvent((float)xpos, (float)ypos);
#endif // _QDEARIMGUI
        }

        static void Local_GlfwMouseButtonCallback(GLFWwindow* a_Window, const int a_Button, const int a_Action, const int a_Mods)
        {
            const QKey qKey = Local_GlfwToQwerkE(a_Button, 0);
            Internal_MouseButton(qKey, static_cast<QKeyState>(GLFW_PRESS == a_Action));

#ifdef _QDEARIMGUI
            ImGui_ImplGlfw_UpdateKeyModifiers(a_Window);

            ImGuiIO& io = ImGui::GetIO();
            if (a_Button >= 0 && a_Button < ImGuiMouseButton_COUNT)
            {
                io.AddMouseButtonEvent(a_Button, a_Action == GLFW_PRESS);
            }
#endif // _QDEARIMGUI
        }

        static void Local_GlfwScrollCallback(GLFWwindow* a_Window, const double a_OffsetX, const double a_OffsetY)
        {
            Internal_MouseScroll(a_OffsetX, a_OffsetY);

#ifdef _QDEARIMGUI
            ImGuiIO& io = ImGui::GetIO();
            io.AddMouseWheelEvent(static_cast<float>(a_OffsetX), static_cast<float>(a_OffsetY));
#endif // _QDEARIMGUI
        }

        static void Local_GlfwJoystickCallback(const int a_JoystickId, const int eventId)
        {
            // #TODO void glfwSetJoystickUserPointer(int jid, void* pointer);
            // or void* glfwGetJoystickUserPointer(int jid);

            switch (eventId)
            {
            case GLFW_CONNECTED: // 262145
                Local_AddGamepad(a_JoystickId);
                break;

            case GLFW_DISCONNECTED: // 262146
                for (size_t i = 0; i < s_GamepadIds.size(); i++)
                {
                    if (a_JoystickId == s_GamepadIds[i])
                    {
                        s_GamepadIds.erase(s_GamepadIds.begin() + i);
                        return;
                    }
                }
                ASSERT(false, "Device disconnected but not registered!");
                break;

            default:
                ASSERT(false, "Unhandled joystick event!");
                break;
            }
        }

        void Input_RegisterGlfwCallbacks(GLFWwindow* window)
        {
            // #TODO Consider moving into Initialize() or otherwise combining the two functions
            // #TODO Handle multiple GLFW windows

            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            // #TODO Set GLFW_JOYSTICK_HAT_BUTTONS at int
            glfwSetKeyCallback(window, Local_GlfwKeyCallback);
            glfwSetCharCallback(window, Local_GlfwCharCallback);

            // For keys as char values: 'a'=97, 'A'=65, mod shift=1 or GLFW_MOD_SHIFT
            // glfwSetCharModsCallback(window, Local_GlfwCharModsCallback);

            glfwSetCursorPosCallback(window, Local_GlfwCursorPositionCallback);
            glfwSetMouseButtonCallback(window, Local_GlfwMouseButtonCallback);
            glfwSetScrollCallback(window, Local_GlfwScrollCallback);

            glfwSetJoystickCallback(Local_GlfwJoystickCallback);
        }

        void Input_UnregisterGlfwCallbacks(GLFWwindow* window)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            glfwSetKeyCallback(window, NULL);
            glfwSetCharCallback(window, NULL);
            // glfwSetCharModsCallback(window, NULL);

            glfwSetCursorPosCallback(window, NULL);
            glfwSetMouseButtonCallback(window, NULL);
            glfwSetScrollCallback(window, NULL);

            glfwSetJoystickCallback(NULL);
        }

#ifdef _QDEARIMGUI
        static void ImGui_ImplGlfw_UpdateKeyModifiers(GLFWwindow * window)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.AddKeyEvent(ImGuiMod_Ctrl, (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS));
            io.AddKeyEvent(ImGuiMod_Shift, (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS));
            io.AddKeyEvent(ImGuiMod_Alt, (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS));
            io.AddKeyEvent(ImGuiMod_Super, (glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS));
        }

        static ImGuiKey ImGui_ImplGlfw_KeyToImGuiKey(int keycode, int scancode)
        {
            IM_UNUSED(scancode);
            switch (keycode)
            {
            case GLFW_KEY_TAB: return ImGuiKey_Tab;
            case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
            case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
            case GLFW_KEY_UP: return ImGuiKey_UpArrow;
            case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
            case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
            case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
            case GLFW_KEY_HOME: return ImGuiKey_Home;
            case GLFW_KEY_END: return ImGuiKey_End;
            case GLFW_KEY_INSERT: return ImGuiKey_Insert;
            case GLFW_KEY_DELETE: return ImGuiKey_Delete;
            case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
            case GLFW_KEY_SPACE: return ImGuiKey_Space;
            case GLFW_KEY_ENTER: return ImGuiKey_Enter;
            case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
            case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
            case GLFW_KEY_COMMA: return ImGuiKey_Comma;
            case GLFW_KEY_MINUS: return ImGuiKey_Minus;
            case GLFW_KEY_PERIOD: return ImGuiKey_Period;
            case GLFW_KEY_SLASH: return ImGuiKey_Slash;
            case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
            case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
            case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
            case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
            // case GLFW_KEY_WORLD_1: return ImGuiKey_Oem102;
            // case GLFW_KEY_WORLD_2: return ImGuiKey_Oem102;
            case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
            case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
            case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
            case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
            case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
            case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
            case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
            case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
            case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
            case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
            case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
            case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
            case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
            case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
            case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
            case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
            case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
            case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
            case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
            case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
            case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
            case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
            case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
            case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
            case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
            case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
            case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
            case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
            case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
            case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
            case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
            case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
            case GLFW_KEY_MENU: return ImGuiKey_Menu;
            case GLFW_KEY_0: return ImGuiKey_0;
            case GLFW_KEY_1: return ImGuiKey_1;
            case GLFW_KEY_2: return ImGuiKey_2;
            case GLFW_KEY_3: return ImGuiKey_3;
            case GLFW_KEY_4: return ImGuiKey_4;
            case GLFW_KEY_5: return ImGuiKey_5;
            case GLFW_KEY_6: return ImGuiKey_6;
            case GLFW_KEY_7: return ImGuiKey_7;
            case GLFW_KEY_8: return ImGuiKey_8;
            case GLFW_KEY_9: return ImGuiKey_9;
            case GLFW_KEY_A: return ImGuiKey_A;
            case GLFW_KEY_B: return ImGuiKey_B;
            case GLFW_KEY_C: return ImGuiKey_C;
            case GLFW_KEY_D: return ImGuiKey_D;
            case GLFW_KEY_E: return ImGuiKey_E;
            case GLFW_KEY_F: return ImGuiKey_F;
            case GLFW_KEY_G: return ImGuiKey_G;
            case GLFW_KEY_H: return ImGuiKey_H;
            case GLFW_KEY_I: return ImGuiKey_I;
            case GLFW_KEY_J: return ImGuiKey_J;
            case GLFW_KEY_K: return ImGuiKey_K;
            case GLFW_KEY_L: return ImGuiKey_L;
            case GLFW_KEY_M: return ImGuiKey_M;
            case GLFW_KEY_N: return ImGuiKey_N;
            case GLFW_KEY_O: return ImGuiKey_O;
            case GLFW_KEY_P: return ImGuiKey_P;
            case GLFW_KEY_Q: return ImGuiKey_Q;
            case GLFW_KEY_R: return ImGuiKey_R;
            case GLFW_KEY_S: return ImGuiKey_S;
            case GLFW_KEY_T: return ImGuiKey_T;
            case GLFW_KEY_U: return ImGuiKey_U;
            case GLFW_KEY_V: return ImGuiKey_V;
            case GLFW_KEY_W: return ImGuiKey_W;
            case GLFW_KEY_X: return ImGuiKey_X;
            case GLFW_KEY_Y: return ImGuiKey_Y;
            case GLFW_KEY_Z: return ImGuiKey_Z;
            case GLFW_KEY_F1: return ImGuiKey_F1;
            case GLFW_KEY_F2: return ImGuiKey_F2;
            case GLFW_KEY_F3: return ImGuiKey_F3;
            case GLFW_KEY_F4: return ImGuiKey_F4;
            case GLFW_KEY_F5: return ImGuiKey_F5;
            case GLFW_KEY_F6: return ImGuiKey_F6;
            case GLFW_KEY_F7: return ImGuiKey_F7;
            case GLFW_KEY_F8: return ImGuiKey_F8;
            case GLFW_KEY_F9: return ImGuiKey_F9;
            case GLFW_KEY_F10: return ImGuiKey_F10;
            case GLFW_KEY_F11: return ImGuiKey_F11;
            case GLFW_KEY_F12: return ImGuiKey_F12;
            case GLFW_KEY_F13: return ImGuiKey_F13;
            case GLFW_KEY_F14: return ImGuiKey_F14;
            case GLFW_KEY_F15: return ImGuiKey_F15;
            case GLFW_KEY_F16: return ImGuiKey_F16;
            case GLFW_KEY_F17: return ImGuiKey_F17;
            case GLFW_KEY_F18: return ImGuiKey_F18;
            case GLFW_KEY_F19: return ImGuiKey_F19;
            case GLFW_KEY_F20: return ImGuiKey_F20;
            case GLFW_KEY_F21: return ImGuiKey_F21;
            case GLFW_KEY_F22: return ImGuiKey_F22;
            case GLFW_KEY_F23: return ImGuiKey_F23;
            case GLFW_KEY_F24: return ImGuiKey_F24;
            default: return ImGuiKey_None;
            }
        }
#endif // _QDEARIMGUI

        constexpr int Local_QwerkEToGlfw(const QGamepad a_QwerkEKey)
        {
            switch (a_QwerkEKey) // GLFW keys defined in glfw3.h, or https://www.glfw.org/docs/latest/group__keys.html
            {
                case e_Gamepad0: return GLFW_GAMEPAD_BUTTON_A;                  // Gamepad device button 0
                case e_Gamepad1: return GLFW_GAMEPAD_BUTTON_B;                  // Gamepad device button 1
                case e_Gamepad2: return GLFW_GAMEPAD_BUTTON_X;                  // Gamepad device button 2
                case e_Gamepad3: return GLFW_GAMEPAD_BUTTON_Y;                  // Gamepad device button 3
                case e_Gamepad4: return GLFW_GAMEPAD_BUTTON_LEFT_BUMPER;        // Gamepad device button 4
                case e_Gamepad5: return GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER;       // Gamepad device button 5
                case e_Gamepad6: return GLFW_GAMEPAD_BUTTON_BACK;               // Gamepad device button 6
                case e_Gamepad7: return GLFW_GAMEPAD_BUTTON_START;              // Gamepad device button 7
                case e_Gamepad8: return GLFW_GAMEPAD_BUTTON_GUIDE;              // Gamepad device button 8
                case e_Gamepad9: return GLFW_GAMEPAD_BUTTON_LEFT_THUMB;         // Gamepad device button 9
                case e_Gamepad10: return GLFW_GAMEPAD_BUTTON_RIGHT_THUMB;       // Gamepad device button 10
                case e_Gamepad11: return GLFW_GAMEPAD_BUTTON_DPAD_UP;           // Gamepad device button 11
                case e_Gamepad12: return GLFW_GAMEPAD_BUTTON_DPAD_RIGHT;        // Gamepad device button 12
                case e_Gamepad13: return GLFW_GAMEPAD_BUTTON_DPAD_DOWN;         // Gamepad device button 13
                case e_Gamepad14: return GLFW_GAMEPAD_BUTTON_DPAD_LEFT;         // Gamepad device button 14
            }

            return QGamepad::e_QGamepadInputMax;
        }

        constexpr int Local_QwerkEToGlfw(const QKey a_QwerkEKey)
        {
            switch (a_QwerkEKey) // GLFW keys defined in glfw3.h, or https://www.glfw.org/docs/latest/group__keys.html
            {
                case e_MouseButton1: return GLFW_MOUSE_BUTTON_1;                // Mouse device button 1
                case e_MouseButton2: return GLFW_MOUSE_BUTTON_2;                // Mouse device button 2
                case e_MouseButton3: return GLFW_MOUSE_BUTTON_3;                // Mouse device button 3
                case e_MouseButton4: return GLFW_MOUSE_BUTTON_4;                // Mouse device button 4
                case e_MouseButton5: return GLFW_MOUSE_BUTTON_5;                // Mouse device button 5
                case e_MouseButton6: return GLFW_MOUSE_BUTTON_6;                // Mouse device button 6
                case e_MouseButton7: return GLFW_MOUSE_BUTTON_7;                // Mouse device button 7
                case e_MouseButton8: return GLFW_MOUSE_BUTTON_8;                // Mouse device button 8

                case e_ShiftL: return GLFW_KEY_LEFT_SHIFT;                      // Left Shift key
                case e_CtrlL: return GLFW_KEY_LEFT_CONTROL;                     // Left CTRL or Control key
                case e_AltL: return GLFW_KEY_LEFT_ALT;                          // Left ALT or Alt key
                case e_SystemL: return GLFW_KEY_LEFT_SUPER;                     // Left System key, like Windows or Win (Microsoft), command or cmd (Apple)
                case e_ShiftR: return GLFW_KEY_RIGHT_SHIFT;                     // Right Shift key
                case e_CtrlR: return GLFW_KEY_RIGHT_CONTROL;                    // Right CTRL or Control key
                case e_AltR: return GLFW_KEY_RIGHT_ALT;                         // Right ALT or Alt key
                case e_SystemR: return GLFW_KEY_RIGHT_SUPER;                    // Right System key, like Windows or Win (Microsoft), command or cmd (Apple)

                case e_Space: return GLFW_KEY_SPACE;                            // Spacebar key

                // #TODO Scancode support
                case e_MediaPlay: return 290;                                   // Play or pause media key
                // #TODO case e_MediaNext: return ;                             // Next or skip forward media key
                // #TODO case e_MediaPrevious: return ;                         // Previous or skip back media key
                case e_MediaVolumeUp: return 304;                               // Volume up media key
                case e_MediaVolumeDown: return 302;                             // Volume down media key
                // #TODO case e_MediaVolumeMute: return ;                       // Volume mute media key
                // #TODO case e_Function: return ;                              // Function or Fn key

                case e_Apostrophe: return GLFW_KEY_APOSTROPHE;                  // ''' or '"' key

                case e_Right: return GLFW_KEY_RIGHT;                            // Right arrow key
                case e_Left: return GLFW_KEY_LEFT;                              // Left arrow key
                case e_Down: return GLFW_KEY_DOWN;                              // Down arrow key
                case e_Up: return GLFW_KEY_UP;                                  // Up arrow key

                case e_Comma: return GLFW_KEY_COMMA;                            // ',' or '<' key
                case e_Minus: return GLFW_KEY_MINUS;                            // '-' or '_' key
                case e_Period: return GLFW_KEY_PERIOD;                          // '.' or '>' key
                case e_SlashForward: return GLFW_KEY_SLASH;                     // '/' or '?' key

                case e_0: return GLFW_KEY_0;                                    // '0' or ')' key
                case e_1: return GLFW_KEY_1;                                    // '1' or '!' key
                case e_2: return GLFW_KEY_2;                                    // '2' or '@' key
                case e_3: return GLFW_KEY_3;                                    // '3' or '#' key
                case e_4: return GLFW_KEY_4;                                    // '4' or '$' key
                case e_5: return GLFW_KEY_5;                                    // '5' or '%' key
                case e_6: return GLFW_KEY_6;                                    // '6' or '^' key
                case e_7: return GLFW_KEY_7;                                    // '7' or '&' key
                case e_8: return GLFW_KEY_8;                                    // '8' or '*' key
                case e_9: return GLFW_KEY_9;                                    // '9' or '(' key

                // #TODO Any return value, or blocked/disabled?
                case e_Any: return GLFW_KEY_A;                                  // Any key. Used in functions to return true for any key event
                case e_CtrlAny: return GLFW_KEY_LEFT_CONTROL;                   // Left or right CTRL or control key
                case e_ShiftAny: return GLFW_KEY_LEFT_SHIFT;                    // Left or right Shift key
                case e_AltAny: return GLFW_KEY_LEFT_ALT;                        // Left or right Alt or alt key

                case e_SemiColon: return GLFW_KEY_SEMICOLON;                    // ';' or ':' key

                case e_Equal: return GLFW_KEY_EQUAL;                            // '=' or '+' key

                case e_A: return GLFW_KEY_A;                                    // 'A' or 'a' key
                case e_B: return GLFW_KEY_B;                                    // 'B' or 'b' key
                case e_C: return GLFW_KEY_C;                                    // 'C' or 'c' key
                case e_D: return GLFW_KEY_D;                                    // 'D' or 'd' key
                case e_E: return GLFW_KEY_E;                                    // 'E' or 'e' key
                case e_F: return GLFW_KEY_F;                                    // 'F' or 'f' key
                case e_G: return GLFW_KEY_G;                                    // 'G' or 'g' key
                case e_H: return GLFW_KEY_H;                                    // 'H' or 'h' key
                case e_I: return GLFW_KEY_I;                                    // 'I' or 'i' key
                case e_J: return GLFW_KEY_J;                                    // 'J' or 'j' key
                case e_K: return GLFW_KEY_K;                                    // 'K' or 'k' key
                case e_L: return GLFW_KEY_L;                                    // 'L' or 'l' key
                case e_M: return GLFW_KEY_M;                                    // 'M' or 'm' key
                case e_N: return GLFW_KEY_N;                                    // 'N' or 'n' key
                case e_O: return GLFW_KEY_O;                                    // 'O' or 'o' key
                case e_P: return GLFW_KEY_P;                                    // 'P' or 'p' key
                case e_Q: return GLFW_KEY_Q;                                    // 'Q' or 'q' key
                case e_R: return GLFW_KEY_R;                                    // 'R' or 'r' key
                case e_S: return GLFW_KEY_S;                                    // 'S' or 's' key
                case e_T: return GLFW_KEY_T;                                    // 'T' or 't' key
                case e_U: return GLFW_KEY_U;                                    // 'U' or 'u' key
                case e_V: return GLFW_KEY_V;                                    // 'V' or 'v' key
                case e_W: return GLFW_KEY_W;                                    // 'W' or 'w' key
                case e_X: return GLFW_KEY_X;                                    // 'X' or 'x' key
                case e_Y: return GLFW_KEY_Y;                                    // 'Y' or 'y' key
                case e_Z: return GLFW_KEY_Z;                                    // 'Z' or 'z' key

                case e_BracketLeft: return GLFW_KEY_LEFT_BRACKET;               // '[' or '{' key
                case e_BackSlash: return GLFW_KEY_BACKSLASH;                    // '\' or '|' key
                case e_BracketRight: return GLFW_KEY_RIGHT_BRACKET;             // ']' or '}' key

                case e_Menu: return GLFW_KEY_MENU;                              // Menu or context or right click key

                case e_Escape: return GLFW_KEY_ESCAPE;                          // Escape key

                case e_Tilde: return GLFW_KEY_GRAVE_ACCENT;                     // '`' or '~' key

                case e_Enter: return GLFW_KEY_ENTER;                            // Enter or Return key
                case e_Tab: return GLFW_KEY_TAB;                                // Tab key
                case e_Backspace: return GLFW_KEY_BACKSPACE;                    // Backspace key
                case e_Insert: return GLFW_KEY_INSERT;                          // Insert key
                case e_Delete: return GLFW_KEY_DELETE;                          // Delete key

                case e_PageUp: return GLFW_KEY_PAGE_UP;                         // Page Up or PgUp key
                case e_PageDown: return GLFW_KEY_PAGE_DOWN;                     // Page Down or PgDn key
                case e_Home: return GLFW_KEY_HOME;                              // Home key
                case e_End: return GLFW_KEY_END;                                // End key

                case e_CapsLock: return GLFW_KEY_CAPS_LOCK;                     // Caps Lock or Caps key
                case e_ScrollLock: return GLFW_KEY_SCROLL_LOCK;                 // Scroll Lock or ScrLk key
                case e_NumLock: return GLFW_KEY_NUM_LOCK;                       // Number Lock or NumLock key
                case e_PrintScreen: return GLFW_KEY_PRINT_SCREEN;               // Print Screen or PrtSc key
                case e_Pause: return GLFW_KEY_PAUSE;                            // Pause key

                case e_F1: return GLFW_KEY_F1;                                  // F1 function key
                case e_F2: return GLFW_KEY_F2;                                  // F2 function key
                case e_F3: return GLFW_KEY_F3;                                  // F3 function key
                case e_F4: return GLFW_KEY_F4;                                  // F4 function key
                case e_F5: return GLFW_KEY_F5;                                  // F5 function key
                case e_F6: return GLFW_KEY_F6;                                  // F6 function key
                case e_F7: return GLFW_KEY_F7;                                  // F7 function key
                case e_F8: return GLFW_KEY_F8;                                  // F8 function key
                case e_F9: return GLFW_KEY_F9;                                  // F9 function key
                case e_F10: return GLFW_KEY_F10;                                // F10 function key
                case e_F11: return GLFW_KEY_F11;                                // F11 function key
                case e_F12: return GLFW_KEY_F12;                                // F12 function key
                case e_F13: return GLFW_KEY_F13;                                // F13 function key
                case e_F14: return GLFW_KEY_F14;                                // F14 function key
                case e_F15: return GLFW_KEY_F15;                                // F15 function key
                case e_F16: return GLFW_KEY_F16;                                // F16 function key
                case e_F17: return GLFW_KEY_F17;                                // F17 function key
                case e_F18: return GLFW_KEY_F18;                                // F18 function key
                case e_F19: return GLFW_KEY_F19;                                // F19 function key
                case e_F20: return GLFW_KEY_F20;                                // F20 function key
                case e_F21: return GLFW_KEY_F21;                                // F21 function key
                case e_F22: return GLFW_KEY_F22;                                // F22 function key
                case e_F23: return GLFW_KEY_F23;                                // F23 function key
                case e_F24: return GLFW_KEY_F24;                                // F24 function key
                case e_F25: return GLFW_KEY_F25;                                // F25 function key

                case e_Pad_0: return GLFW_KEY_KP_0;                             // '0' or Ins number pad key
                case e_Pad_1: return GLFW_KEY_KP_1;                             // '1' or End number pad key
                case e_Pad_2: return GLFW_KEY_KP_2;                             // '2' or Down number pad key
                case e_Pad_3: return GLFW_KEY_KP_3;                             // '3' or PgDn number pad key
                case e_Pad_4: return GLFW_KEY_KP_4;                             // '4' or Left number pad key
                case e_Pad_5: return GLFW_KEY_KP_5;                             // '5' number pad key
                case e_Pad_6: return GLFW_KEY_KP_6;                             // '6' or Right number pad key
                case e_Pad_7: return GLFW_KEY_KP_7;                             // '7' or Home number pad key
                case e_Pad_8: return GLFW_KEY_KP_8;                             // '8' or Up number pad key
                case e_Pad_9: return GLFW_KEY_KP_9;                             // '9' or PgUp number pad key
                case e_Pad_Decimal: return GLFW_KEY_KP_DECIMAL;                 // '.' or Del number pad key
                case e_Pad_Division: return GLFW_KEY_KP_DIVIDE;                 // '/' number pad key
                case e_Pad_Multiply: return GLFW_KEY_KP_MULTIPLY;               // '*' number pad key
                case e_Pad_Minus: return GLFW_KEY_KP_SUBTRACT;                  // '-' number pad key
                case e_Pad_Plus: return GLFW_KEY_KP_ADD;                        // '+' number pad key
                case e_Pad_Return: return GLFW_KEY_KP_ENTER;                    // Enter or Return number pad key
                case e_Pad_Equal: return GLFW_KEY_KP_EQUAL;                     // '=' number pad key

                // #TODO Need QKey entry case ?: return GLFW_KEY_WORLD_1; // 161 non-US #1
                // #TODO Need QKey entry case ?: return GLFW_KEY_WORLD_2; // 162 non-US #2

                // e_MAX,                          // Invalid entry and end of enum
            }

            LOG_CRITICAL("Unsupported QKey {0}!", a_QwerkEKey);
            // ASSERT(false, "Unsupported QKey key!");
            return QKey::e_QKeyMax;
        }

        constexpr QKey Local_GlfwToQwerkE(const int a_GlfwKey, int a_Scancode)
        {
            switch (a_GlfwKey) // GLFW keys defined in glfw3.h, or https://www.glfw.org/docs/latest/group__keys.html
            {
                case GLFW_MOUSE_BUTTON_1: return e_MouseButton1;                // Mouse device button 1
                case GLFW_MOUSE_BUTTON_2: return e_MouseButton2;                // Mouse device button 2
                case GLFW_MOUSE_BUTTON_3: return e_MouseButton3;                // Mouse device button 3
                case GLFW_MOUSE_BUTTON_4: return e_MouseButton4;                // Mouse device button 4
                case GLFW_MOUSE_BUTTON_5: return e_MouseButton5;                // Mouse device button 5
                case GLFW_MOUSE_BUTTON_6: return e_MouseButton6;                // Mouse device button 6
                case GLFW_MOUSE_BUTTON_7: return e_MouseButton7;                // Mouse device button 7
                case GLFW_MOUSE_BUTTON_8: return e_MouseButton8;                // Mouse device button 8

                case GLFW_KEY_SPACE:            return QKey::e_Space;           // 32 ' '

                case GLFW_KEY_APOSTROPHE:       return QKey::e_Apostrophe;      // 39 '''

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

            LOG_CRITICAL("Unsupported GLFW key {0} or scancode {1}!", a_GlfwKey, a_Scancode);
            // ASSERT(false, "Unsupported GLFW key!");
            return QKey::e_QKeyMax;
        }

    }

}
#endif
