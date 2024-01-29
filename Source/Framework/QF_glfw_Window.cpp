#include "QF_glfw_Window.h"

#include <assert.h>

#include "Libraries/imgui/QC_imgui.h"

#include "QF_CallbackFunctions.h"
#include "QF_Window.h"
#include "QF_Windows.h"

namespace QwerkE {

    void priv_close_callback(GLFWwindow* window)
    {
        glfw_Window* glfw_window = (glfw_Window*)Windows::GetWindow(0);
        glfw_window->SetClosing(true);
    }

    glfw_Window::glfw_Window(int windowWidth, int windowHeight, const char* windowTitle) : Window(windowWidth, windowHeight, windowTitle)
    {
        // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // #ifdef __APPLE__
        // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
    // #endif

        m_Window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
        assert(m_Window);
        glfwSwapInterval(0); // TODO: Add VSynch control and toggling
        glfwMakeContextCurrent(m_Window);
        glfwFocusWindow(m_Window);
        m_IsFocused = true;
        SetupCallbacks(m_Window); // TODO: Window won't respond to clicking corner 'x'... sometimes?
        glfwSetWindowCloseCallback(m_Window, priv_close_callback);

        ImGui_ImplGlfw_InitForOpenGL(m_Window, false);
    }

    glfw_Window::~glfw_Window()
    {
        glfwWindowShouldClose(m_Window); // May have issues waiting so long to close s_Window. Try to call Close() when it is requested
        glfwDestroyWindow(m_Window);
    }

    void glfw_Window::Render()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        SwapBuffers();
    }

    void glfw_Window::NewFrame()
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
}
