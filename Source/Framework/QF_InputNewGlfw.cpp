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

    }

}
#endif
