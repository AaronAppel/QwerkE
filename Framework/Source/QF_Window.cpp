#include "QF_Window.h"

#include "Libraries/glew/glew.h"
#include "Libraries/glfw/glfw3.h"
// #include "Libraries/imgui/QC_imgui.h"

// #include "QF_Assets.h"
// #include "QF_Debug.h"
// #include "QF_Defines.h"
// #include "QF_FileSystem.h"
// #include "QF_FileUtilities.h"
// #include "QF_Log.h"
// #include "QF_Renderer.h"
// #include "QF_Scene.h"
// #include "QF_Scenes.h"
// #include "QF_Settings.h"

const char* g_WindowTitle = "QwerkEngine";

// From https://stackoverflow.com/questions/11335301/glfw-get-screen-height-width
// void Window::CenterTheWindow(){
// GLFWmonitor* monitor = glfwGetPrimaryMonitor();
// const GLFWvidmode* mode = glfwGetVideoMode(monitor);
// glfwSetWindowPos(m_Window, (mode->width - m_Width) / 2, (mode->height - m_Height) / 2);
// }

namespace QwerkE {

    namespace Window {

        bool s_windowIsMinimized = false;
        bool s_closeRequested = false;
        vec2 s_aspectRatio = vec2(16.f, 9.f);

    #ifdef _QGLFW3
        GLFWwindow* s_window = nullptr;

        void local_error_callback(int error, const char* description)
        {
            // LOG_ERROR(description);
        }

        void local_framebuffer_size_callback(GLFWwindow* window, int width, int height)
        {
            // Renderer::OnWindowResized( vec2(width, height) );
        }

        void local_window_resized_callback(GLFWwindow* window, int width, int height)
        {
            // Renderer::OnWindowResized(vec2(width, height));
        }

        void local_close_callback(GLFWwindow* window)
        {
            s_closeRequested = true;
        }

        void local_window_iconify_callback(GLFWwindow* window, int iconified)
        {
            s_windowIsMinimized = iconified == 1;
        }

        void local_CheckGlfwErrors();
    #endif

        void Window::Initialize()
        {
    #ifdef _QGLFW3
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

            vec2 resolution = vec2(1600, 900); // #TODO Fix hard code
            s_window = glfwCreateWindow((int)resolution.x, (int)resolution.y, g_WindowTitle, NULL, NULL);
            if (!s_window)
            {
                local_CheckGlfwErrors();
            }
            glfwSwapInterval(0); // TODO: Add VSynch control and toggling
            glfwMakeContextCurrent(s_window);
            glfwFocusWindow(s_window);

            glfwSetErrorCallback(local_error_callback);
            glfwSetFramebufferSizeCallback(s_window, local_framebuffer_size_callback);
            glfwSetWindowCloseCallback(s_window, local_close_callback);
            glfwSetWindowSizeCallback(s_window, local_window_resized_callback);
            glfwSetWindowIconifyCallback(s_window, local_window_iconify_callback);
            // glfwSetDropCallback(s_window, local_file_drop_callback);

    #endif

    #ifdef _QGLEW
            const GLenum glewInitCode = glewInit();
            ASSERT(glewInitCode == GLEW_OK, "Error loading GLEW!");
    #endif

    #ifdef _QDEARIMGUI
            ImGuiContext* context = ImGui::CreateContext();
            ASSERT(context != nullptr, "Error loading imgui!");
            ImGui::SetCurrentContext(context);

            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

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
    #ifdef _QGLFW3
            glfwWindowShouldClose(s_window);
            glfwDestroyWindow(s_window);
    #endif

    #ifdef _QDEARIMGUI
    #ifdef _QGLFW3
            ImGui_ImplGlfw_Shutdown();
    #endif
    #ifdef _QOPENGL
            ImGui_ImplOpenGL3_Shutdown();
    #endif
            ImGui::DestroyContext();
    #endif

    #ifdef _QGLFW3
            glfwTerminate();
    #endif
        }

        void Window::Render()
        {
    #ifdef _QDEARIMGUI
            if (s_windowIsMinimized)
            {
                ImGui::EndFrame();
                ImGui::UpdatePlatformWindows();
                return;
            }

            ImGui::Render();

    #if defined(_QOPENGL) && defined(_QDEARIMGUI)
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    #endif

            ImGuiIO io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
    #ifdef _QGLFW3
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                glfwMakeContextCurrent(backup_current_context);
    #endif
            }
    #endif
            SwapBuffers();
        }

        void Window::SwapBuffers()
        {
    #ifdef _QGLFW3
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
            int width, height;
            glfwGetWindowSize(s_window, &width, &height);
            return vec2(width, height);
        }

        vec2 Window::GetAspectRatio()
        {
            return s_aspectRatio;
        }

        void* Window::GetContext()
        {
            return s_window;
        }

        bool IsMinimized()
        {
            return s_windowIsMinimized;
        }

    #ifdef _QGLFW3
        void local_CheckGlfwErrors()
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

}
