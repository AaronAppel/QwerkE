#include "QF_Input.h"

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#ifdef _QGLFW3
#include "Libraries/glfw/glfw3.h"
#else
#error Define library!
#endif

#include "QF_Constants.h"
#include "QF_Log.h"
// #include "QF_Renderer.h"

namespace QwerkE {

    namespace Input     {

#ifdef _QGLFW3
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
        static void char_callback(GLFWwindow* window, unsigned int codePoint);
        static void char_mods_callback(GLFWwindow* window, unsigned int codepoint, int mods) { }
        static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
        static void cursor_enter_callback(GLFWwindow* window, int entered) { }
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

        void SetupCallbacks(GLFWwindow* window)
        {
            glfwSetKeyCallback(window, key_callback);
            glfwSetScrollCallback(window, scroll_callback);
            glfwSetCharCallback(window, char_callback);
            glfwSetCharModsCallback(window, char_mods_callback);
            glfwSetCursorPosCallback(window, cursor_position_callback);
            glfwSetMouseButtonCallback(window, mouse_button_callback);
        }

        void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
        {
            if (key == -1)
            {
                switch (scancode)
                {
                case 256: // Media key volume up/down
                    break;

                case 290: // Media key play/pause
                    break;
                }
            }

            const bool keyIsRepeating = action == GLFW_REPEAT;
            const bool keyIsReleased = action == GLFW_RELEASE;
            const eKeys qwerkEeKey = GLFWToQwerkEKey(key);

            OnKeyEvent(qwerkEeKey, (eKeyState)!keyIsReleased);

#ifdef _QDEARIMGUI
            // ImGui::CaptureKeyboardFromApp(true);

            ImGuiIO& io = ImGui::GetIO();
            switch (qwerkEeKey)
            {
            case eKeys::eKeys_LCTRL:
                io.AddKeyEvent(ImGuiKey::ImGuiKey_LeftCtrl, !keyIsReleased);
                break;

            case eKeys::eKeys_RCTRL:
                io.AddKeyEvent(ImGuiKey::ImGuiKey_RightCtrl, !keyIsReleased);
                break;

            default:
                break;
            }

            // #TODO See if AddKeyEvent handles modifiers
            // io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
            // io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
            // io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
            // io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
#endif
        }

        void char_callback(GLFWwindow* window, unsigned int codePoint)
        {
#ifdef _QDEARIMGUI
            if (codePoint > 0 && codePoint < 0x10000)
            {
                ImGui::GetIO().AddInputCharacter(codePoint);
            }
#endif
        }

        void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
        {
            OnMouseMove((float)xpos, (float)-ypos);
        }

        void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
        {
            // ImGui::CaptureMouseFromApp(true);
            if (action == GLFW_PRESS)
            {
                OnMouseClick(GLFWToQwerkEKey(button), eKeyState::eKeyState_Press);
#ifdef _QDEARIMGUI
                ImGuiIO& io = ImGui::GetIO();
                io.AddMouseButtonEvent(button, true);
#endif
            }
            else if (action == GLFW_RELEASE)
            {
                OnMouseClick(GLFWToQwerkEKey(button), eKeyState::eKeyState_Release);
#ifdef _QDEARIMGUI
                ImGuiIO& io = ImGui::GetIO();
                io.AddMouseButtonEvent(button, false);
#endif
            }
        }

        void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
        {
            OnMouseScroll((float)xoffset, (float)yoffset);

#ifdef _QDEARIMGUI
            ImGuiIO& io = ImGui::GetIO();
            io.MouseWheelH += (float)xoffset;
            io.MouseWheel += (float)yoffset;
#endif
        }
#endif

    }

}
