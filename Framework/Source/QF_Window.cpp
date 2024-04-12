#include "QF_Window.h"

#ifdef _QGLEW
#include "Libraries/glew/glew.h"
#endif

#ifdef _QGLFW3
#define GLFW_EXPOSE_NATIVE_WIN32
#include "Libraries/glfw/glfw3.h"
#endif

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#include "QF_Assets.h"
#include "QF_Debug.h"
#include "QF_Files.h"
#include "QF_Paths.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Settings.h"

const char* g_WindowTitle = "QwerkEngine";

namespace QwerkE {

    namespace Window {

        bool s_windowIsMinimized = false;
        bool s_closeRequested = false;
        float s_aspectRatio = 16.f / 9.f;

        CallBacks::FramebufferResizedCallback* s_FramebufferResizedCallback = nullptr;
        CallBacks::WindowResizedCallback* s_WindowResizedCallback = nullptr;
        CallBacks::KeyCallback* s_KeyCallback = nullptr;

    #ifdef _QGLFW3
        GLFWwindow* s_window = nullptr;

        void local_file_drop_callback(GLFWwindow* window, int fileCount, const char** filePaths);
        void local_CheckGlfwErrors();

        void local_error_callback(int error, const char* description)
        {
            // LOG_ERROR(description);
        }

        void local_framebuffer_size_callback(GLFWwindow* window, int width, int height)
        {
            // Renderer::OnFramebufferResized( vec2(width, height) );
            if (s_FramebufferResizedCallback)
            {
                (*s_FramebufferResizedCallback)(width, height);
            }
        }

        void local_window_resized_callback(GLFWwindow* window, int width, int height)
        {
            // Renderer::OnWindowResized(vec2(width, height));
            if (s_WindowResizedCallback)
            {
                (*s_WindowResizedCallback)(width, height);
            }
        }

        void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
        {
            if (s_KeyCallback)
            {
                (*s_KeyCallback)(key, scancode, action, mode);
            }
        }

        void local_close_callback(GLFWwindow* window)
        {
            s_closeRequested = true;
        }

        void local_window_iconify_callback(GLFWwindow* window, int iconified)
        {
            s_windowIsMinimized = iconified == 1;
        }
    #endif

        void Window::Initialize()
        {
    #ifdef _QGLFW3
            const int glfwInitCode = glfwInit();
            ASSERT(glfwInitCode == 1, "Error loading GLFW step!");

#if defined(_QOPENGL) && !defined(_QBGFX)
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Using OpenGL 4.3
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
            glfwWindowHint(GLFW_SAMPLES, 8);
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#elif defined(_QBGFX)
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Create future GLFW windows without a OpenGL contexts
#else
#error Define graphics library!
#endif
            glfwWindowHint(GLFW_RED_BITS, 8);
            glfwWindowHint(GLFW_GREEN_BITS, 8);
            glfwWindowHint(GLFW_BLUE_BITS, 8);
            glfwWindowHint(GLFW_ALPHA_BITS, 8);
            glfwWindowHint(GLFW_DEPTH_BITS, 0);
            glfwWindowHint(GLFW_STENCIL_BITS, 8);

            // GLFWmonitor* glfwPrimaryMonitor = glfwGetPrimaryMonitor(); // #BUG Bricks PC when going fullscreen

            const EngineSettings& engineSettings = Settings::GetEngineSettings();
            s_window = glfwCreateWindow((int)engineSettings.windowWidthPixels, (int)engineSettings.windowHeightPixels, g_WindowTitle, NULL, NULL);
            if (!s_window)
            {
                local_CheckGlfwErrors();
            }
            s_aspectRatio = (float)engineSettings.windowWidthPixels / (float)engineSettings.windowHeightPixels;

            glfwSwapInterval(0); // #TODO Use engineSettings.vSyncEnabled
            glfwMakeContextCurrent(s_window);
            const bool wantWindowFocus = false; // #TODO Add option for window focus (I don't like auto focus)
            if (wantWindowFocus)
            {
                glfwFocusWindow(s_window);
            }

            glfwSetErrorCallback(local_error_callback);
            glfwSetFramebufferSizeCallback(s_window, local_framebuffer_size_callback);
            glfwSetWindowCloseCallback(s_window, local_close_callback);
            glfwSetWindowSizeCallback(s_window, local_window_resized_callback);
            glfwSetWindowIconifyCallback(s_window, local_window_iconify_callback);
            glfwSetDropCallback(s_window, local_file_drop_callback);

            glfwSetKeyCallback(s_window, key_callback);
    #endif

    #ifdef _QGLEW
            const GLenum glewInitCode = glewInit();
            ASSERT(glewInitCode == GLEW_OK, "Error loading GLEW!");
    #endif

            // imgui handled by bgfx for now. Look to remove this deprecated code
    //#ifdef _QDEARIMGUI
    //        ImGuiContext* context = ImGui::CreateContext();
    //        ASSERT(context != nullptr, "Error loading imgui!");
    //        ImGui::SetCurrentContext(context);
    //
    //        ImGuiIO& io = ImGui::GetIO();
    //        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    //        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    //        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    //
    //        ImGui::StyleColorsDark();
    //
    //        ImGuiStyle& style = ImGui::GetStyle();
    //        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    //        {
    //            style.WindowRounding = 0.0f;
    //            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    //        }
    //
    //        ImGui_ImplOpenGL3_Init("#version 410");
    //
    //        ImGui_ImplGlfw_InitForOpenGL(s_window, false);
    //#endif
        }

        void Window::Shutdown()
        {
    #ifdef _QGLFW3
            glfwWindowShouldClose(s_window);
            glfwDestroyWindow(s_window);

            // imgui handled by bgfx for now. Look to remove this deprecated code
    // #ifdef _QDEARIMGUI
    //         ImGui_ImplGlfw_Shutdown();
    //
    // #ifdef _QOPENGL
    //         ImGui_ImplOpenGL3_Shutdown();
    // #endif
    //         ImGui::DestroyContext();
    // #endif

            glfwTerminate();
    #endif
        }

        void Window::ImGuiRender() // #TODO Move to Renderer
        {
                // imgui handled by bgfx for now. Look to remove this deprecated code
    // #ifdef _QDEARIMGUI
    //         if (s_windowIsMinimized)
    //         {
    //             ImGui::EndFrame();
    //             ImGui::UpdatePlatformWindows();
    //             return;
    //         }
    //
    //         ImGui::Render();
    //
    // #if defined(_QOPENGL) && defined(_QDEARIMGUI)
    //         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // #endif
    //
    //         ImGuiIO io = ImGui::GetIO();
    //         if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    //         {
    //             ImGui::UpdatePlatformWindows();
    //             ImGui::RenderPlatformWindowsDefault();
    // #ifdef _QGLFW3
    //             GLFWwindow* backup_current_context = glfwGetCurrentContext();
    //             glfwMakeContextCurrent(backup_current_context);
    // #endif
    //         }
    // #endif
        }

        void Window::NewFrame()
        {
            glfwPollEvents();
            // imgui handled by bgfx for now. Look to remove this deprecated code
//#ifdef _QDEARIMGUI
//            ImGui_ImplOpenGL3_NewFrame();
//            ImGui_ImplGlfw_NewFrame();
//            ImGui::NewFrame();
//#endif
        }

        void Window::RequestClose()
        {
            s_closeRequested = true;
        }

        bool Window::CloseRequested()
        {
            return s_closeRequested;
        }

        const vec2f Window::GetSize()
        {
            int width, height;
            glfwGetWindowSize(s_window, &width, &height);
            return vec2f(width, height);
        }

        float Window::GetAspectRatio()
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

        void RegisterFramebufferResizedCallback(CallBacks::FramebufferResizedCallback* framebufferResizedCallback)
        {
            s_FramebufferResizedCallback = framebufferResizedCallback;
        }

        void RegisterWindowResizedCallback(CallBacks::WindowResizedCallback* windowResizedCallback)
        {
            s_WindowResizedCallback = windowResizedCallback;
        }

        void RegisterKeyCallback(CallBacks::KeyCallback* keyCallback)
        {
            s_KeyCallback = keyCallback;
        }

    #ifdef _QGLFW3
        // #TODO Call editor or add an editor callback so window doesn't have file drop logic in release/non-editor build.
        void local_file_drop_callback(GLFWwindow* window, int fileCount, const char** filePaths)
        {
            for (int i = 0; i < fileCount; i++)
            {
                const Path fileName = Files::FileName(filePaths[i]);
                const Path fileExtension = Files::FileExtension(filePaths[i]);

                if (strcmp(fileExtension.string().c_str(), ".qscene") == 0)
                {
                    std::string scenefilePath = Paths::ScenesDir() + fileName.string();
                    if (Files::Exists(scenefilePath.c_str()))
                    {
                        if (Scene* newScene = Scenes::CreateSceneFromFile(scenefilePath.c_str()))
                        {
                            Scenes::SetCurrentScene(newScene);
                            Assets::AddToRegistry(newScene->GetGuid(), newScene->GetSceneName());
                        }
                    }
                }
                else
                {
                    LOG_WARN("Drag file type unsupported: {0}", fileExtension.string().c_str());
                }
            }
        }

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
