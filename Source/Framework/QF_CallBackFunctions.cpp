#include "QF_CallbackFunctions.h"

#include "Libraries/glew/GL/glew.h"
#include "Libraries/glfw/GLFW/glfw3.h"

#include "QF_Log.h"
#include "QF_FileUtilities.h"
#include "QF_FileSystem.h"
#include "QF_Resources.h"
#include "../QwerkE_Framework/Source/Core/Graphics/GraphicsUtilities/GraphicsHelpers.h"
#include "../QwerkE_Framework/Source/Core/Graphics/DataTypes/Texture.h"

namespace QwerkE {

    void window_resized_callback(GLFWwindow* window, int width, int height) { } // #TODO Handle window resized callback
    void window_closed_callback(GLFWwindow* window) { } // #TODO Handle window closed callback

    void error_callback(int error, const char* description);

    void SetupCallbacks(GLFWwindow* window)
    {
        glfwSetWindowSizeCallback(window, window_resized_callback);
        glfwSetWindowCloseCallback(window, window_closed_callback);

        glfwSetErrorCallback(error_callback);

        glfwSetDropCallback(window, file_drop_callback);
    }

    void error_callback(int error, const char* description)
    {
        LOG_ERROR(description);
    }

    void file_drop_callback(GLFWwindow* window, int count, const char** paths)
    {
        // Path of file drag and dropped onto this window
        // TODO: Handle file drop correctly. This is hacked in for testing purposes at the moment.

        std::string dropFileExtensionStr = GetFileExtension(*paths).c_str(); // TODO: Avoid extra variable due to heap deallocation
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
