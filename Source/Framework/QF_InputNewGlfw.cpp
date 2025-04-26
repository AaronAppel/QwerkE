#ifdef _QGLFW3
#include "Libraries/glfw/glfw3.h"

// #include "QF_InputNew.h"

#include "QF_eKeys.h"
#include "QF_Window.h"

namespace QwerkE {

    namespace Input {

        // Stage 1 //
        // Minimal (MVP)
        // Polling API

        extern void OnKeyEvent_New(eKeys key, eKeyState state);
        extern eKeys GLFWToQwerkEKey(int key);

        struct Xbit
        {
            Xbit(unsigned char value)
            {
                bitValue = value;
            }
            unsigned int bitValue : 8; // x bit unsigned integer
        };
        using HistoryIndexPointer = Xbit; // Supports rollover

        extern const u8 s_HistoryBufferSize;
        extern eKeys s_HistoryBufferKeys[256];
        extern eKeyState s_HistoryBufferStates[256];
        extern HistoryIndexPointer s_CurrentHistoryIndex;
        extern HistoryIndexPointer s_LastFrameStartIndex;
        extern HistoryIndexPointer s_LastFrameEndIndex;

        int QwerkEKeyToGLFW(eKeys key) // #TODO Find key mapping solution
        {
            if (key < 0 || key >= GLFW_KEY_LAST)
            {
                LOG_ERROR("{0} Key code {1} out of range!", __FUNCTION__, key);
                return GLFW_KEY_LAST;
            }
            return key;
        }

        void Local_GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

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

        bool KeyState(eKeys a_Key);

        void Local_GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
        {
            const eKeys qwerkEeKey = GLFWToQwerkEKey(key);

            ASSERT(eKeys::eKeys_MAX != qwerkEeKey, "Unsupported key!");
            if (GLFW_REPEAT != action) // #TODO Support GLFW_REPEAT
            {
                OnKeyEvent_New(qwerkEeKey, (eKeyState)(action));
            }
        }

        bool Key(const eKeys a_Key, const eKeyState a_KeyState)
        {
            u8 index = s_CurrentHistoryIndex.bitValue - 1;
            u8 iterationsRemaining =
                s_CurrentHistoryIndex.bitValue > s_LastFrameEndIndex.bitValue ?
                s_CurrentHistoryIndex.bitValue - s_LastFrameEndIndex.bitValue :
                s_LastFrameEndIndex.bitValue - s_CurrentHistoryIndex.bitValue;

            while (iterationsRemaining > 0)
            {
                if (a_Key == s_HistoryBufferKeys[index] &&
                    a_KeyState == s_HistoryBufferStates[index])
                {
                    return true;
                }
                --index;
                --iterationsRemaining;
            }
            return false;
        }

        bool KeyDown(const eKeys a_Key)
        {
            return Key(a_Key, eKeyState::eKeyState_Press);
        }

        bool KeyUp(const eKeys a_Key)
        {
            return Key(a_Key, eKeyState::eKeyState_Release);
        }

        bool KeyState(const eKeys a_Key)
        {
            return GLFW_PRESS == glfwGetKey(static_cast<GLFWwindow*>(Window::GetContext()), QwerkEKeyToGLFW(a_Key));
        }

    }

}
#endif
