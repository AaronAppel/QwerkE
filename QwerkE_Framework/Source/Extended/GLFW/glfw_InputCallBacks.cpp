#include "../../Core/Input/Input.h"

#ifdef dearimgui
#include "../../Libraries/imgui/imgui.h"
#endif

#include "QF_QwerkE_Global_Constants.h"

#include "../../Core/Input/Mouse.h"
#include "../../Core/Input/Keyboard.h"
#include "../../Core/Graphics/Renderer.h"

namespace QwerkE {

#ifdef GLFW3

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    void char_callback(GLFWwindow* window, unsigned int codePoint);
    void char_mods_callback(GLFWwindow* window, unsigned int codepoint, int mods) { }
    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    void cursor_enter_callback(GLFWwindow* window, int entered) { }
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void joystick_callback(int joy, int event) { }

    void Input::SetupCallbacks(GLFWwindow* window)
    {
        glfwSetKeyCallback(window, key_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetCharCallback(window, char_callback);
        glfwSetCharModsCallback(window, char_mods_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetJoystickCallback(joystick_callback);
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if (action == GLFW_PRESS)
        {
            Input::ProcessKeyEvent(Input::GLFWToQwerkEKey(key), eKeyState::eKeyState_Press);
        }
        else if (action == GLFW_RELEASE)
        {
            Input::ProcessKeyEvent(Input::GLFWToQwerkEKey(key), eKeyState::eKeyState_Release);
        }

#ifdef dearimgui
        ImGuiIO& io = ImGui::GetIO();

        if (action == GLFW_PRESS)
        {
            io.KeysDown[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            io.KeysDown[key] = false;
        }

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
#endif
    }

    void char_callback(GLFWwindow* window, unsigned int codePoint)
    {
#ifdef dearimgui
        if (codePoint > 0 && codePoint < 0x10000)
        {
            ImGui::GetIO().AddInputCharacter((unsigned short)codePoint);
        }
#endif
    }

    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        Input::ProcessMouseMove((float)xpos, (float)-ypos);
    }

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            Input::ProcessMouseClick(Input::GLFWToQwerkEKey(button), eKeyState::eKeyState_Press);
        }
        else if (action == GLFW_RELEASE)
        {
            Input::ProcessMouseClick(Input::GLFWToQwerkEKey(button), eKeyState::eKeyState_Release);
        }
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
#ifdef dearimgui
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += (float)xoffset;
        io.MouseWheel += (float)yoffset;

#endif
    }

}
#endif
