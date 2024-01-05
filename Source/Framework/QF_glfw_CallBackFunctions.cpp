#include "QF_glfw_CallbackFunctions.h"

#include "../../../Libraries/glew/GL/glew.h"
#include "../../../Libraries/glfw/GLFW/glfw3.h"

#include "../QwerkE_Framework/Source/Debug/Log/Log.h"
#include "QF_FileUtilities.h"
#include "QF_FileSystem.h"
#include "../QwerkE_Framework/Source/Core/Resources/Resources.h"
#include "../QwerkE_Framework/Source/Core/Graphics/GraphicsUtilities/GraphicsHelpers.h"
#include "../QwerkE_Framework/Source/Core/Graphics/DataTypes/Texture.h"

namespace QwerkE {

    void window_moved_callback(GLFWwindow* window, int x, int y) { }
    void window_resized_callback(GLFWwindow* window, int width, int height) { }
    void window_closed_callback(GLFWwindow* window) { }
    void window_refreshed_callback(GLFWwindow* window) { }
    void window_focused_callback(GLFWwindow* window, int isFocused) { }
    void window_iconified_callback(GLFWwindow* window, int isIconified) { }

    void framebuffer_resized_callback(GLFWwindow* window, int x, int y) { }

    void error_callback(int error, const char* description);

    void file_dropped_callback(GLFWwindow* window, int count, const char** paths);

    void SetupCallbacks(GLFWwindow* window)
    {
        glfwSetWindowPosCallback(window, window_moved_callback);
        glfwSetWindowSizeCallback(window, window_resized_callback);
        glfwSetWindowCloseCallback(window, window_closed_callback);
        glfwSetWindowRefreshCallback(window, window_refreshed_callback);
        glfwSetWindowFocusCallback(window, window_focused_callback);
        glfwSetWindowIconifyCallback(window, window_iconified_callback);

        glfwSetFramebufferSizeCallback(window, framebuffer_resized_callback);

        glfwSetErrorCallback(error_callback);

        glfwSetDropCallback(window, file_dropped_callback);
    }

    void error_callback(int error, const char* description)
    {
        LOG_ERROR(description);
    }

    void file_dropped_callback(GLFWwindow* window, int count, const char** paths)
    {
        const std::string dropFileExtensionStr = GetFileExtension(*paths).c_str();
        const char* dropFileExtension = dropFileExtensionStr.c_str();

        for (int i = 0; i < count; i++)
        {
            if (strcmp(dropFileExtension, "png") == 0 || strcmp(GetFileExtension(*paths).c_str(), "jpg") == 0)
            {
                // TODO: Ask Resources or another file to load the asset
                GLuint result = Load2DTexture(*paths, 0);
                if (result != 0)
                {
                    Texture* texture = new Texture();
                    texture->s_Handle = result;
                    texture->s_FileName = GetFileNameWithExt(*paths);
                    Resources::AddTexture(GetFileNameNoExt(*paths).c_str(), texture);
                }
            }
            else if (strcmp(dropFileExtension, "fbx") == 0 || strcmp(GetFileExtension(*paths).c_str(), "obj") == 0)
            {
                FileSystem::LoadModelFileToMeshes(*paths);
            }
            else
            {
                LOG_WARN("Drag file type unsupported: {0}", dropFileExtension);
            }
        }
    }

}
