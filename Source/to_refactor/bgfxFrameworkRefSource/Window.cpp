#include "Window.h"

#include <assert.h>
#include <stdio.h>

#ifdef _QGLFW3
#define GLFW_EXPOSE_NATIVE_WIN32
#include "Libraries/glfw/glfw3.h"
#endif

namespace Window {

    const char* s_WindowTitle = "bgfx Framework";
    bool s_windowIsMinimized = false;
    bool s_closeRequested = false;

#ifdef _QGLFW3
    GLFWwindow* s_window = nullptr;

    void local_CheckGlfwErrors();

    void local_close_callback(GLFWwindow* window)
    {
        s_closeRequested = true;
    }

    void local_window_iconify_callback(GLFWwindow* window, int iconified)
    {
        s_windowIsMinimized = iconified == 1;
    }

    void local_error_callback(int error, const char* description)
    {
        local_CheckGlfwErrors();
        // std::cout << description << "\n";
    }
#endif

    void Window::Initialize(int windowWidth, int windowHeight)
    {
#ifdef _QGLFW3
        const int glfwInitCode = glfwInit();
        assert(glfwInitCode == GLFW_TRUE, "Error loading GLFW step!");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Create future GLFW windows without a OpenGL contexts

        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);
        glfwWindowHint(GLFW_DEPTH_BITS, 0);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);

        s_window = glfwCreateWindow(windowWidth, windowHeight, s_WindowTitle, NULL, NULL);
        if (!s_window)
        {
            local_CheckGlfwErrors();
        }

        glfwMakeContextCurrent(s_window);
        glfwSetErrorCallback(local_error_callback); // Error callback triggered above by glfwMakeContextCurrent when no context is set

        glfwFocusWindow(s_window);

        glfwSetWindowCloseCallback(s_window, local_close_callback);
        glfwSetWindowIconifyCallback(s_window, local_window_iconify_callback);
#endif
    }

    void Window::Shutdown()
    {
#ifdef _QGLFW3
        glfwWindowShouldClose(s_window);
        glfwDestroyWindow(s_window);
        glfwTerminate();
#endif
    }

    void Window::NewFrame()
    {
        glfwPollEvents();
    }

    void Window::RequestClose()
    {
        s_closeRequested = true;
    }

    bool Window::CloseRequested()
    {
        return s_closeRequested;
    }

    void Window::GetWindowSize(int* x, int* y)
    {
        glfwGetWindowSize(s_window, x, y);
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
        const int glfwErrorCode = glfwGetError(NULL);
        const char* const functionString = __FUNCTION__;

        switch (glfwErrorCode)
        {
        case GLFW_NOT_INITIALIZED:
            printf("%s GLFW error GLFW_NOT_INITIALIZED!", functionString);
            break;
        case GLFW_NO_CURRENT_CONTEXT:
            printf("%s GLFW error GLFW_NO_CURRENT_CONTEXT!", functionString);
            break;
        case GLFW_INVALID_ENUM:
            printf("%s GLFW error GLFW_INVALID_ENUM!", functionString);
            break;
        case GLFW_INVALID_VALUE:
            printf("%s GLFW error GLFW_INVALID_VALUE!", functionString);
            break;
        case GLFW_OUT_OF_MEMORY:
            printf("%s GLFW error GLFW_OUT_OF_MEMORY!", functionString);
            break;
        case GLFW_API_UNAVAILABLE:
            printf("%s GLFW error GLFW_API_UNAVAILABLE!", functionString);
            break;
        case GLFW_VERSION_UNAVAILABLE:
            printf("%s GLFW error GLFW_VERSION_UNAVAILABLE!", functionString);
            break;
        case GLFW_PLATFORM_ERROR:
            printf("%s GLFW error GLFW_PLATFORM_ERROR!", functionString);
            break;
        case GLFW_FORMAT_UNAVAILABLE:
            printf("%s GLFW error GLFW_FORMAT_UNAVAILABLE!", functionString);
            break;
        case GLFW_NO_WINDOW_CONTEXT:
            printf("%s GLFW error GLFW_NO_WINDOW_CONTEXT!", functionString);
            break;
        case GLFW_NO_ERROR:
            printf("%s No error was detected, but GLFW was not able to create a window object!", functionString);
            break;
        }
    }
#endif

}
