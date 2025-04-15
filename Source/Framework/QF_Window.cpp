#include "QF_Window.h"

#ifdef _QBGFXFRAMEWORK
#include <bgfxFramework/bgfx_utils.h>
#endif

#ifdef _QGLFW3
#define GLFW_EXPOSE_NATIVE_WIN32
#include "Libraries/glfw/glfw3.h"
#endif

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QwerkE_imgui.h"
#endif

#include "QF_Assets.h"
#include "QF_Debug.h"
#include "QF_Files.h"
#include "QF_Mesh.h"
#include "QF_Paths.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Shader.h"

#include "QF_Mirror.h" // #TODO Review how to include reflection. Needs basis or forced include precompiled header?

#include "../Source/Editor/QE_Projects.h" // #TODO Remove from QF_* domain
#include "../Source/Editor/QE_Settings.h" // #TODO Remove from QF_* domain

const char* g_WindowTitle = "QwerkEngine";

namespace QwerkE {

    namespace Window {

        bool s_windowIsMinimized = false;
        bool s_closeRequested = false;
        float s_aspectRatio = 16.f / 9.f;

        Callbacks::FramebufferResizedCallback* s_FramebufferResizedCallback = nullptr;
        Callbacks::WindowResizedCallback* s_WindowResizedCallback = nullptr;
        Callbacks::KeyCallback* s_KeyCallback = nullptr;

#if 1 // #TODO Omit from retail builds
        Callbacks::FileDropCallback* s_FileDropCallback = nullptr;
#endif

    #ifdef _QGLFW3
        GLFWwindow* s_window = nullptr;

#if 1 // #TODO Omit from retail builds
        void local_FileDropCallback(GLFWwindow* window, int fileCount, const char** filePaths);
#endif
        void local_CheckGlfwErrors();

        // #TODO Consistent naming for events, like local_OnError maybe? Match with other callbacks like in Input::
        void local_ErrorCallback(int error, const char* description)
        {
            // LOG_ERROR(description);
        }

        void local_FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
        {
            // Renderer::OnFramebufferResized( vec2(width, height) );
            if (s_FramebufferResizedCallback)
            {
                (*s_FramebufferResizedCallback)(width, height);
            }
        }

        void local_WindowResizedCallback(GLFWwindow* window, int width, int height)
        {
            // Renderer::OnWindowResized(vec2(width, height));
            if (s_WindowResizedCallback)
            {
                (*s_WindowResizedCallback)(width, height);
            }
        }

        void local_KeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
        {
            if (s_KeyCallback)
            {
                (*s_KeyCallback)(key, scancode, action, mode);
            }
        }

        void local_CloseCallback(GLFWwindow* window)
        {
            s_closeRequested = true;
        }

        void local_WindowIconifyCallback(GLFWwindow* window, int iconified)
        {
            s_windowIsMinimized = iconified == 1;
        }

        void local_window_pos_callback(GLFWwindow* window, int xpos, int ypos)
        {
        }
    #endif

        void Window::Initialize(u16 windowWidth, u16 windowHeight)
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
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Create future GLFW windows without OpenGL contexts
#else
#error Define graphics library!
#endif
            glfwWindowHint(GLFW_RED_BITS, 8);
            glfwWindowHint(GLFW_GREEN_BITS, 8);
            glfwWindowHint(GLFW_BLUE_BITS, 8);
            glfwWindowHint(GLFW_ALPHA_BITS, 8);
            glfwWindowHint(GLFW_DEPTH_BITS, 0);
            glfwWindowHint(GLFW_STENCIL_BITS, 8);

            auto engineSettings = Settings::GetEngineSettings();
            glfwWindowHint(GLFW_POSITION_X, engineSettings.windowOpenPositionX);
            glfwWindowHint(GLFW_POSITION_Y, engineSettings.windowOpenPositionY);
            //glfwSetWindowPos(window, 100, 100);

            // GLFWmonitor* glfwPrimaryMonitor = glfwGetPrimaryMonitor(); // #BUG Bricks PC when going fullscreen

            s_window = glfwCreateWindow((int)windowWidth, (int)windowHeight, g_WindowTitle, NULL, NULL);
            if (!s_window)
            {
                local_CheckGlfwErrors();
            }
            s_aspectRatio = (float)windowHeight / (float)windowHeight;

            glfwSwapInterval(0); // #TODO Use engineSettings.vSyncEnabled
            glfwMakeContextCurrent(s_window);
            const bool wantWindowFocus = false; // #TODO Add option for window focus (I don't like auto focus)
            if (wantWindowFocus)
            {
                glfwFocusWindow(s_window);
            }

            glfwSetErrorCallback(local_ErrorCallback);
            glfwSetFramebufferSizeCallback(s_window, local_FrameBufferSizeCallback);
            glfwSetWindowCloseCallback(s_window, local_CloseCallback);
            glfwSetWindowSizeCallback(s_window, local_WindowResizedCallback);
            glfwSetWindowIconifyCallback(s_window, local_WindowIconifyCallback);
            glfwSetWindowPosCallback(s_window, local_window_pos_callback);
            glfwSetDropCallback(s_window, local_FileDropCallback);

            glfwSetKeyCallback(s_window, local_KeyEventCallback);
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

        const vec2f Window::GetSize()
        {
            int width, height;
            glfwGetWindowSize(s_window, &width, &height);
            return vec2f(width, height);
        }

        const vec2f Window::GetPosition()
        {
            int xpos, ypos;
            glfwGetWindowPos(s_window, &xpos, &ypos);
            return vec2f(xpos, ypos);
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

        void RegisterFramebufferResizedCallback(Callbacks::FramebufferResizedCallback* framebufferResizedCallback)
        {
            s_FramebufferResizedCallback = framebufferResizedCallback;
        }

        void RegisterWindowResizedCallback(Callbacks::WindowResizedCallback* windowResizedCallback)
        {
            s_WindowResizedCallback = windowResizedCallback;
        }

        void RegisterKeyCallback(Callbacks::KeyCallback* keyCallback)
        {
            s_KeyCallback = keyCallback;
        }

#if 1 // #TODO Omit from retail builds
        void RegisterFileDropCallback(Callbacks::FileDropCallback* fileDropCallback)
        {
            s_FileDropCallback = fileDropCallback;
        }
#endif

#if 1 // #TODO Omit from retail builds
        // #TODO Call editor or add an editor callback so window doesn't have file drop logic in release/non-editor build.
        void local_FileDropCallback(GLFWwindow* window, int fileCount, const char** filePaths)
        {
            for (int i = 0; i < fileCount; i++)
            {
                if (s_FileDropCallback)
                {
                    (*s_FileDropCallback)(filePaths[i]);
                }

                // #TODO Deprecate or move code below
                const Path fileName = Files::FileName(filePaths[i]);
                const Path fileExtension = Files::FileExtension(filePaths[i]);

                if (strcmp(fileExtension.string().c_str(), ".qscene") == 0)
                {
                    std::string scenefilePath = Paths::Scene(fileName.string().c_str());
                    if (Files::Exists(scenefilePath.c_str()))
                    {
                        if (Scene* existingScene = Scenes::GetScene(fileName.u8string()))
                        {
                            Assets::AddToRegistry(Mirror::TypeId<Scene>(), existingScene->GetGuid(), existingScene->GetSceneName());
                        }
                        else if (Scene* newScene = Scenes::CreateSceneFromFile(scenefilePath.c_str()))
                        {
                            // #NOTE Scene transition change removes next line
                            Scenes::SetCurrentScene(newScene);
                            Assets::AddToRegistry(Mirror::TypeId<Scene>(), newScene->GetGuid(), newScene->GetSceneName());
                        }
                    }
                }
                else if (strcmp(fileExtension.string().c_str(), ".obj") == 0)
                {
                    std::string meshFilePath = Paths::Mesh(fileName.string().c_str());
                    if (Files::Exists(meshFilePath.c_str()))
                    {
                        Assets::AddToRegistry(Mirror::TypeId<Mesh>(), GUID(), fileName.string());
                    }
                }
                else if (strcmp(fileExtension.string().c_str(), ".bin") == 0)
                {
                    // #NOTE Binary mesh files starts with "VB", and binry shaders start with "FSH" or "VSH"
                    if (Files::Exists(Paths::Mesh(fileName.string().c_str()).c_str()))
                    {
                        Assets::AddToRegistry(Mirror::TypeId<bgfxFramework::Mesh>(), GUID(), fileName.string());
                    }
                    else if (Files::Exists(Paths::Shader(fileName.string().c_str()).c_str()))
                    {
                        Assets::AddToRegistry(Mirror::TypeId<Shader>(), GUID(), fileName.string());
                    }
                }
                else
                {
                    LOG_WARN("Drag file type unsupported: {0}", fileExtension.string().c_str());
                }
            }
        }
#endif

#ifdef _QGLFW3 // #TODO Omit from retail builds
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
