#include "QF_Windows.h"

#include "Libraries/glew/GL/glew.h"
#include "Libraries/glfw/GLFW/glfw3.h"
#include "Libraries/imgui/imgui.h"

#include "QF_Log.h"
#include "QF_Window.h"

namespace QwerkE {

    std::vector<Window*> Windows::m_Windows;
    unsigned char Windows::m_lastFocusedWindowIndex = 0;

    void Windows::Initialize()
    {
        const int glfwInitCode = glfwInit();
        if (glfwInitCode)
        {
            LOG_TRACE("GLFW Loaded,");

            // TODO: Setup proper testWindow hints
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //using OpenGL 4.3
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

            GLFWwindow* testWindow;
            testWindow = glfwCreateWindow(100, 100, "Test", NULL, NULL);
            if (!testWindow)
            {
                LOG_ERROR("Error loading GLFW step 2!");

                /*int code = glfwGetError(NULL);

                switch (code)
                {
                case GLFW_NOT_INITIALIZED:
                    ConsolePrint("\nGLFW error GLFW_NOT_INITIALIZED!\n");
                    break;
                case GLFW_NO_CURRENT_CONTEXT:
                    ConsolePrint("\nGLFW error GLFW_NO_CURRENT_CONTEXT!\n");
                    break;
                case GLFW_INVALID_ENUM:
                    ConsolePrint("\nGLFW error GLFW_INVALID_ENUM!\n");
                    break;
                case GLFW_INVALID_VALUE:
                    ConsolePrint("\nGLFW error GLFW_INVALID_VALUE!\n");
                    break;
                case GLFW_OUT_OF_MEMORY:
                    ConsolePrint("\nGLFW error GLFW_OUT_OF_MEMORY!\n");
                    break;
                case GLFW_API_UNAVAILABLE:
                    ConsolePrint("\nGLFW error GLFW_API_UNAVAILABLE!\n");
                    break;
                case GLFW_VERSION_UNAVAILABLE:
                    ConsolePrint("\nGLFW error GLFW_VERSION_UNAVAILABLE!\n");
                    break;
                case GLFW_PLATFORM_ERROR:
                    ConsolePrint("\nGLFW error GLFW_PLATFORM_ERROR!\n");
                    break;
                case GLFW_FORMAT_UNAVAILABLE:
                    ConsolePrint("\nGLFW error GLFW_FORMAT_UNAVAILABLE!\n");
                    break;
                case GLFW_NO_WINDOW_CONTEXT:
                    ConsolePrint("\nGLFW error GLFW_NO_WINDOW_CONTEXT!\n");
                    break;
                case GLFW_NO_ERROR:
                    ConsolePrint("\nNo error was detected, but GLFW was not able to create a testWindow object!\n");
                    break;
                }*/
            }
            else
            {
                glfwMakeContextCurrent(testWindow);
                const GLenum glewInitCode = glewInit();
                if (glewInitCode == GLEW_OK)
                {
                    LOG_TRACE("GLEW Loaded,");
                }
                else
                {
                    LOG_ERROR("Error loading GLEW!");
                }

                glfwDestroyWindow(testWindow); // #TODO Review testWindow destroy
            }
        }
        else
        {
            LOG_ERROR("Error loading GLFW step 1!");
        }
        // imgui GLFW
    }

    void Windows::Shutdown()
    {
       /* int size = m_Windows.size();
        for (int i = 0; i < size; i++)
        {
            delete m_Windows.at(i);
        }*/

        ImGui_ImplGlfw_Shutdown();
        glfwTerminate();
    }

    void Windows::AddWindow(Window* window)
    {
        m_Windows.push_back(window);
    }

    Window* Windows::GetWindow(int windowID)
    {
        if (windowID >= 0 && windowID < m_Windows.size())
        {
            return m_Windows.at(windowID);
        }
    }

    const Window* Windows::GetLastFocusedWindow()
    {
        if (m_lastFocusedWindowIndex < m_Windows.size())
        {
            return m_Windows.at(m_lastFocusedWindowIndex);
        }
        return nullptr;
    }

}
