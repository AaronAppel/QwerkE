#include "QF_Window.h"

#include "Libraries/glew/GL/glew.h"
#include "Libraries/glfw/GLFW/glfw3.h"
#include "Libraries/imgui/QC_imgui.h"

#include "QF_CallBackFunctions.h" // #TODO Implement
#include "QF_Debug.h"
#include "QF_Defines.h"
#include "QF_Log.h"
#include "QF_Renderer.h"

const char* g_WindowTitle = "QwerkEngine";

namespace QwerkE
{
    bool s_windowIsMaximized = false;
    bool s_closeRequested = false;
    vec2 s_resolution = vec2(1600.f, 900.f);
    vec2 s_aspectRatio = vec2(16.f, 9.f);

#ifdef GLFW3
    GLFWwindow* s_window = nullptr;

    void priv_close_callback(GLFWwindow* window)
    {
        s_closeRequested = true;
    }

    void priv_CheckGlfwErrors();
#endif

    void Window::Initialize()
    {
#ifdef GLFW3
        const int glfwInitCode = glfwInit();
        ASSERT(glfwInitCode == 1, "Error loading GLFW step!");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Using OpenGL 4.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 8);

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);
        glfwWindowHint(GLFW_DEPTH_BITS, 0);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);

        GLFWmonitor* glfwPrimaryMonitor = glfwGetPrimaryMonitor();

        s_window = glfwCreateWindow((int)s_resolution.x, (int)s_resolution.y, g_WindowTitle, NULL, NULL);
        if (!s_window)
        {
            priv_CheckGlfwErrors();
        }
        glfwSwapInterval(0); // TODO: Add VSynch control and toggling
        glfwMakeContextCurrent(s_window);
        glfwFocusWindow(s_window);
        SetupCallbacks(s_window); // TODO: Window won't respond to clicking corner 'X'... sometimes?
        glfwSetWindowCloseCallback(s_window, priv_close_callback);
#endif

#ifdef GLEW
        const GLenum glewInitCode = glewInit();
        ASSERT(glewInitCode == GLEW_OK, "Error loading GLEW!");
#endif

#ifdef dearimgui
        ImGuiContext* context = ImGui::CreateContext();
        ASSERT(context != nullptr, "Error loading imgui!");
        ImGui::SetCurrentContext(context);

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

        //ImGui::CaptureMouseFromApp(true);
        //ImGui::CaptureKeyboardFromApp(true);

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplOpenGL3_Init("#version 410");

        ImGui_ImplGlfw_InitForOpenGL(s_window, false);
#endif
    }

    void Window::Shutdown()
    {
#ifdef GLFW3
        glfwWindowShouldClose(s_window);
        glfwDestroyWindow(s_window);
#endif

#ifdef dearimgui
        ImGui::DestroyContext();
        ImGui_ImplGlfw_Shutdown();
#endif

#ifdef GLFW3
        glfwTerminate();
#endif
    }

    void Window::Render()
    {
#ifdef dearimgui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
#ifdef GLFW3
            glfwMakeContextCurrent(backup_current_context);
#endif
        }
#endif
        SwapBuffers();
    }

    void Window::SwapBuffers()
    {
#ifdef GLFW3
        glfwSwapBuffers(s_window);
#endif
    }

    void Window::NewFrame()
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Window::ToggleMaximized()
    {
        // #TODO Add more functionality with :
        // glfwShowWindow
        // glfwHideWindow
        // glfwFocusWindow
        // glfwSetInputMode for handling "Sticky Keys"

        if (s_windowIsMaximized)
        {
            glfwRestoreWindow(s_window);
        }
        else
        {
            glfwMaximizeWindow(s_window);
        }
        s_windowIsMaximized = !s_windowIsMaximized;
    }

    void Window::RequestClose()
    {
        s_closeRequested = true;
    }

    bool Window::CloseRequested()
    {
        return s_closeRequested;
    }

    vec2 Window::GetResolution()
    {
        return s_resolution;
    }

    vec2 Window::GetAspectRatio()
    {
        return s_aspectRatio;
    }

    void* Window::GetContext()
    {
        return s_window;
    }

#ifdef GLFW3
    void priv_CheckGlfwErrors()
    {
        int glfwErrorCode = glfwGetError(NULL);

        switch (glfwErrorCode)
        {
        case GLFW_NOT_INITIALIZED:
            LOG_ERROR("{0} GLFW error GLFW_NOT_INITIALIZED!", __FUNCTION__);
            break;
        case GLFW_NO_CURRENT_CONTEXT:
            LOG_ERROR("{0} GLFW error GLFW_NO_CURRENT_CONTEXT!", __FUNCTION__);
            break;
        case GLFW_INVALID_ENUM:
            LOG_ERROR("{0} GLFW error GLFW_INVALID_ENUM!", __FUNCTION__);
            break;
        case GLFW_INVALID_VALUE:
            LOG_ERROR("{0} GLFW error GLFW_INVALID_VALUE!", __FUNCTION__);
            break;
        case GLFW_OUT_OF_MEMORY:
            LOG_ERROR("{0} GLFW error GLFW_OUT_OF_MEMORY!", __FUNCTION__);
            break;
        case GLFW_API_UNAVAILABLE:
            LOG_ERROR("{0} GLFW error GLFW_API_UNAVAILABLE!", __FUNCTION__);
            break;
        case GLFW_VERSION_UNAVAILABLE:
            LOG_ERROR("{0} GLFW error GLFW_VERSION_UNAVAILABLE!", __FUNCTION__);
            break;
        case GLFW_PLATFORM_ERROR:
            LOG_ERROR("{0} GLFW error GLFW_PLATFORM_ERROR!", __FUNCTION__);
            break;
        case GLFW_FORMAT_UNAVAILABLE:
            LOG_ERROR("{0} GLFW error GLFW_FORMAT_UNAVAILABLE!", __FUNCTION__);
            break;
        case GLFW_NO_WINDOW_CONTEXT:
            LOG_ERROR("{0} GLFW error GLFW_NO_WINDOW_CONTEXT!", __FUNCTION__);
            break;
        case GLFW_NO_ERROR:
            LOG_ERROR("{0} No error was detected, but GLFW was not able to create a window object!", __FUNCTION__);
            break;
        }
        ASSERT(false, "Error loading GLFW!");
    }
#endif

}
