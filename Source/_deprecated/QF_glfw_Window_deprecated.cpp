#include "QF_glfw_Window.h"

#include <assert.h>

#include "Libraries/imgui/QC_imgui.h"

#include "QF_CallbackFunctions.h"
#include "QF_Debug.h"
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
        m_Window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
        ASSERT(m_Window, "Error creating window!");
        glfwSwapInterval(0); // TODO: Add VSynch control and toggling
        glfwFocusWindow(m_Window);
        SetupCallbacks(m_Window); // TODO: Window won't respond to clicking corner 'X'... sometimes?
        glfwSetWindowCloseCallback(m_Window, priv_close_callback);

        ImGui_ImplGlfw_InitForOpenGL(m_Window, false);
    }

    glfw_Window::~glfw_Window()
    {
        glfwWindowShouldClose(m_Window);
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
