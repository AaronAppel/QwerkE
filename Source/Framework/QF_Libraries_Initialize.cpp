#include "QF_Libraries_Initialize.h"

#include "QF_Libraries_Include.h"

#include "QF_Debug.h"
#include "QF_Defines.h"
#include "QF_Platform.h"
#include "QF_Log.h"
#include "QF_ConfigHelper.h"

namespace QwerkE {

    static bool s_IsInitialized = false;

    bool Libs_Setup()
    {
        // #TODO This logic tests to see if libraries can load, and then unloads them.
        // This logic should exist in other systems.
        // There may be some benefit to having test cases to see when a library fails to load,
        // but that can be done in the specific system that uses it.

        ASSERT(!s_IsInitialized, "Libraries are already initialized!");

        ConfigData configData = ConfigHelper::GetConfigData();
        // if (configData.systems.AudioEnabled) {}

        bool errorFree = true;
        LOG_TRACE("{0}: Initializing libraries...", __FUNCTION__);

        FT_Library freeType;
        if (FT_Init_FreeType(&freeType))
        {
            LOG_ERROR("Error loading freetype2!");
            errorFree = false;
        }
        else
        {
            LOG_TRACE("Freetype Loaded,");
            FT_Done_FreeType(freeType);
        }

        if (!glfwInit())
        {
            LOG_ERROR("Error loading GLFW step 1!");
            errorFree = false;
        }
        else
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
                errorFree = false;

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
                if (glewInit() != GLEW_OK)
                {
                    LOG_ERROR("Error loading GLEW!");
                    errorFree = false;
                }
                else
                {
                    LOG_TRACE("GLEW Loaded,");
                }

                glfwDestroyWindow(testWindow); // #TODO Review testWindow destroy
            }
        }

        ImGuiContext* context = ImGui::CreateContext();
        if (context == nullptr)
        {
            LOG_ERROR("Error loading imgui!");
            errorFree = false;
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

        if (errorFree)
        {
            LOG_TRACE("{0}: Libraries initialized successfully", __FUNCTION__);
        }
        else
        {
            LOG_CRITICAL("{0}: Error loading libraries!", __FUNCTION__);
        }

        s_IsInitialized = errorFree;
        return errorFree;
    }

    void Libs_TearDown()
    {
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwTerminate();
        Log::Shutdown();

        s_IsInitialized = false;
    }

}
