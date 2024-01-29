#include "QF_Windows.h"

#include "Libraries/glew/GL/glew.h"
#include "Libraries/glfw/GLFW/glfw3.h"
#include "Libraries/imgui/QC_imgui.h"

#include "QF_glfw_Window.h"

#include "QF_Log.h"
#include "QF_Renderer.h"
#include "QF_Window.h"

const char* g_WindowTitle = "QwerkEngine";

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

        ImGuiContext* context = ImGui::CreateContext();
        if (context == nullptr)
        {
            LOG_CRITICAL("Error loading imgui!");
        }
        else
        {
            ImGui::SetCurrentContext(context);

            LOG_TRACE("imgui Loaded,");
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
        }

        glfw_Window* newWindow = new glfw_Window(Renderer::g_WindowWidth, Renderer::g_WindowHeight, g_WindowTitle);
        m_Windows.push_back(newWindow);
    }

    void Windows::Shutdown()
    {
        for (size_t i = 0; i < m_Windows.size(); i++)
        {
            delete m_Windows.at(i);
        }

        ImGui::DestroyContext();
        ImGui_ImplGlfw_Shutdown(); // #TODO Fix inconsistent compile error
        glfwTerminate();
    }

    void Windows::Render()
    {
        for (size_t i = 0; i < m_Windows.size(); i++)
        {
            m_Windows[i]->Render();
        }
    }

    Window* Windows::GetWindow(uint windowID)
    {
        if (windowID >= 0 && windowID < m_Windows.size())
        {
            return m_Windows.at(windowID);
        }
        return nullptr;
    }

}
