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
#include "QF_Framework.h"
#include "QF_Mesh.h"
#include "QF_Paths.h"
#include "QF_Renderer.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Shader.h"

#include "QF_Mirror.h" // #TODO Review how to include reflection. Needs basis or forced include precompiled header?


const char* g_WindowTitle = "QwerkEngine";

namespace QwerkE {

#ifdef _QGLFW3
    namespace Input {
        extern void Input_RegisterGlfwCallbacks(GLFWwindow* window);
    }
#endif

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
        GLFWwindow* s_GlfwWindow = nullptr;

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
            Renderer::OnWindowResized((u32) width, (u32)height);
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

        void Initialize(const Framework::StartUpData& startUpData)
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

            glfwWindowHint(GLFW_POSITION_X, startUpData.windowOpenPositionX);
            glfwWindowHint(GLFW_POSITION_Y, startUpData.windowOpenPositionY);

            // GLFWmonitor* glfwPrimaryMonitor = glfwGetPrimaryMonitor(); // #BUG Bricks PC when going fullscreen

            s_GlfwWindow = glfwCreateWindow((int)startUpData.windowWidth, (int)startUpData.windowHeight, g_WindowTitle, NULL, NULL);
            ASSERT(s_GlfwWindow, "Critical error creating GLFWWindow*!");
            if (!s_GlfwWindow)
            {
                local_CheckGlfwErrors();
                // #TODO Exit app properly
            }
            s_aspectRatio = (float)startUpData.windowHeight / (float)startUpData.windowHeight;

            glfwSwapInterval(startUpData.vSyncEnabled);
            glfwMakeContextCurrent(s_GlfwWindow);

            if (startUpData.windowAutoFocusOnStart)
            {
                glfwFocusWindow(s_GlfwWindow);
            }

#if _QGLFW3
            Input::Input_RegisterGlfwCallbacks(s_GlfwWindow);
#endif // _QGLFW3

            glfwSetErrorCallback(local_ErrorCallback);
            glfwSetFramebufferSizeCallback(s_GlfwWindow, local_FrameBufferSizeCallback);
            glfwSetWindowCloseCallback(s_GlfwWindow, local_CloseCallback);
            glfwSetWindowSizeCallback(s_GlfwWindow, local_WindowResizedCallback);
            glfwSetWindowIconifyCallback(s_GlfwWindow, local_WindowIconifyCallback);
            glfwSetWindowPosCallback(s_GlfwWindow, local_window_pos_callback);
            glfwSetDropCallback(s_GlfwWindow, local_FileDropCallback);

            // #TODO glfwSetWindowRefreshCallback();
            // #TODO glfwSetWindowMaximizeCallback();
            // #TODO glfwSetWindowContentScaleCallback();
            // #TODO glfwSetWindowFocusCallback(); // Also add io.AddFocusEvent(focused != 0); from: https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_glfw.cpp#L422
            // #TODO glfwSetMonitorCallback();
            // #TODO glfwSetCursorEnterCallback(); // Also add io.AddMousePosEvent(-FLT_MAX, -FLT_MAX); from: https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_glfw.cpp#L445

            // #NOTE Overridden in QF_glfw_InputCallBacks.cpp::SetupCallbacks(41)
            // glfwSetKeyCallback(s_window, local_KeyEventCallback);

            // #TODO Also checkout ImGui_ImplGlfw_UpdateMonitors() in https://github.com/ocornut/imgui/blob/docking/backends/imgui_impl_glfw.cpp#L931
    #endif
        }

        void Window::Shutdown()
        {
    #ifdef _QGLFW3
            glfwWindowShouldClose(s_GlfwWindow);
            glfwDestroyWindow(s_GlfwWindow);

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
            glfwGetWindowSize(s_GlfwWindow, &width, &height);
            return vec2f(width, height);
        }

        const vec2f Window::GetPosition()
        {
            int xpos, ypos;
            glfwGetWindowPos(s_GlfwWindow, &xpos, &ypos);
            return vec2f(xpos, ypos);
        }

        float Window::GetAspectRatio()
        {
            return s_aspectRatio;
        }

        void* Window::GetContext()
        {
            return s_GlfwWindow;
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
                            Assets::AddToRegistry(Mirror::IdForType<Scene>(), existingScene->GetGuid(), existingScene->GetSceneName());
                        }
                        else if (Scene* newScene = Scenes::CreateSceneFromFile(scenefilePath.c_str()))
                        {
                            // #NOTE Scene transition change removes next line
                            Scenes::SetCurrentScene(newScene);
                            Assets::AddToRegistry(Mirror::IdForType<Scene>(), newScene->GetGuid(), newScene->GetSceneName());
                        }
                    }
                }
                else if (strcmp(fileExtension.string().c_str(), ".obj") == 0)
                {
                    // #TODO When dragging source files, should the binary versions be looked for instead?
                    std::string meshFilePath = Paths::Mesh(fileName.string().c_str());
                    if (Files::Exists(meshFilePath.c_str()))
                    {
                        Assets::AddToRegistry(Mirror::IdForType<Mesh>(), GUID(), fileName.string());
                    }
                }
                else if (strcmp(fileExtension.string().c_str(), ".vert") == 0)
                {
                    // #TODO When dragging source files, should the binary versions be looked for instead?
                    LOG_WARN(".vert support not yet implemented: {0}", fileExtension.string().c_str());
                }
                else if (strcmp(fileExtension.string().c_str(), ".frag") == 0)
                {
                    // #TODO When dragging source files, should the binary versions be looked for instead?
                    LOG_WARN(".frag support not yet implemented: {0}", fileExtension.string().c_str());
                }
                else if (strcmp(fileExtension.string().c_str(), ".bin") == 0)
                {
                    Buffer buffer = Files::LoadFile(filePaths[i]);
                    if (buffer.SizeInBytes() >= 3)
                    {
                        if (buffer.Data()[0] == 'V' && buffer.Data()[1] == 'B') // Mesh
                        {
                            // Mesh
                            // #TODO Update to use Meshes/bin folder
                            if (Files::Exists(Paths::MeshBin(fileName.string().c_str()).c_str()))
                            {
                                Assets::AddToRegistry(Mirror::IdForType<Mesh>(), GUID(), fileName.string());
                            }
                        }
                        else if (buffer.Data()[0] == 'V' && buffer.Data()[1] == 'S' && buffer.Data()[2] == 'H') // Vertex shader
                        {
                            if (Files::Exists(Paths::ShaderBin(fileName.string().c_str()).c_str()))
                            {
                                Assets::AddToRegistry(Mirror::IdForType<Shader>(), GUID(), fileName.string());
                            }
                        }
                        else if (buffer.Data()[0] == 'F' && buffer.Data()[1] == 'S' && buffer.Data()[2] == 'H') // Fragment shader
                        {
                            if (Files::Exists(Paths::ShaderBin(fileName.string().c_str()).c_str()))
                            {
                                Assets::AddToRegistry(Mirror::IdForType<Shader>(), GUID(), fileName.string());
                            }
                        }
                        else
                        {
                            LOG_WARN("Invalid file type ID data: {0}", fileName.string().c_str());
                        }
                    }
                    else
                    {
                        LOG_WARN("Invalid file data: {0}", fileName.string().c_str());
                    }
                }
                else if (strcmp(fileExtension.string().c_str(), ".png") == 0)
                {
                    if (Files::Exists(Paths::Texture(fileName.string().c_str()).c_str()))
                    {
                        Assets::AddToRegistry(Mirror::IdForType<Texture>(), GUID(), fileName.string());
                    }
                }
                else if (strcmp(fileExtension.string().c_str(), ".wav") == 0)
                {
                    if (Files::Exists(Paths::Sound(fileName.string().c_str()).c_str()))
                    {
                        Assets::AddToRegistry(Mirror::IdForType<Sound>(), GUID(), fileName.string());
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
