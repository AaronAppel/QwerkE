#include "QF_Input.h"

#ifdef dearimgui
#include "Libraries/imgui/QC_imgui.h"
#endif

#include "QF_Constants.h"
#include "QF_Renderer.h"

namespace QwerkE {

#ifdef GLFW3
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    void char_callback(GLFWwindow* window, unsigned int codePoint);
    void char_mods_callback(GLFWwindow* window, unsigned int codepoint, int mods) { }
    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    void cursor_enter_callback(GLFWwindow* window, int entered) { }
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    void Input::SetupCallbacks(GLFWwindow* window)
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
        if (action == GLFW_REPEAT)
        {
            int bp = 0;
        }
        const bool keyIsReleased = action == GLFW_RELEASE;
        const eKeys eKey = Input::GLFWToQwerkEKey(key); // 343
        Input::OnKeyEvent(eKey, (eKeyState)!keyIsReleased);

#if defined(dearimgui)
        // ImGui::CaptureKeyboardFromApp(true);

        ImGuiIO& io = ImGui::GetIO();
        switch (eKey)
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
#ifdef dearimgui
        if (codePoint > 0 && codePoint < 0x10000)
        {
            ImGui::GetIO().AddInputCharacter(codePoint);
        }
#endif
    }

    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        Input::OnMouseMove((float)xpos, (float)-ypos);
    }

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        // ImGui::CaptureMouseFromApp(true);
        if (action == GLFW_PRESS)
        {
            Input::OnMouseClick(Input::GLFWToQwerkEKey(button), eKeyState::eKeyState_Press);
#ifdef dearimgui
            ImGuiIO& io = ImGui::GetIO();
            io.AddMouseButtonEvent(button, true);
#endif
        }
        else if (action == GLFW_RELEASE)
        {
            Input::OnMouseClick(Input::GLFWToQwerkEKey(button), eKeyState::eKeyState_Release);
#ifdef dearimgui
            ImGuiIO& io = ImGui::GetIO();
            io.AddMouseButtonEvent(button, false);
#endif
        }
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        Input::OnMouseScroll((float)xoffset, (float)yoffset);

#ifdef dearimgui
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += (float)xoffset;
        io.MouseWheel += (float)yoffset;
#endif
    }
#endif

}
