#include "QF_CallbackFunctions.h"

#include "Libraries/glew/GL/glew.h"
#include "Libraries/glfw/GLFW/glfw3.h"

#include "QF_FileSystem.h"
#include "QF_FileUtilities.h"
#include "QF_GraphicsHelpers.h"
#include "QF_Log.h"
#include "QF_Resources.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Serialization.h"
#include "QF_Texture.h"

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
        // #TODO Use File:: utils

        for (int i = 0; i < count; i++)
        {
            uPtr<char> fullFileName = SmartFileName(paths[i], true);
            uPtr<char> dropFileExtensionStr = SmartFileExtension(paths[i]);

            if (strcmp(dropFileExtensionStr.get(), "png") == 0 || strcmp(dropFileExtensionStr.get(), "jpg") == 0)
            {
                if (!Resources::TextureExists(fullFileName.get()))
                {
                    Resources::GetTextureFromPath(*paths);
                }
            }
            else if (strcmp(dropFileExtensionStr.get(), "fbx") == 0 || strcmp(dropFileExtensionStr.get(), "obj") == 0)
            {
                if (!Resources::MeshExists(fullFileName.get()))
                {
                    File::LoadModelFileToMeshes(*paths);
                }
            }
            else if (strcmp(dropFileExtensionStr.get(), "ssch") == 0)
            {
                if (!Resources::ShaderProgramExists(fullFileName.get()))
                {
                    Resources::GetShaderProgramFromPath(*paths);
                }
            }
            else if (strcmp(dropFileExtensionStr.get(), "qscene") == 0)
            {
                const std::vector<Scene*>& scenes = Scenes::LookAtScenes();
                bool addScene = true;
                for (size_t i = 0; i < scenes.size(); i++)
                {
                    if (strcmp(fullFileName.get(), scenes[i]->GetSceneName().c_str()) == 0)
                    {
                        LOG_WARN("Scene already exists: {0}", fullFileName.get());
                        addScene = false;
                        break;
                    }
                }

                if (addScene)
                {
                    Scene* newScene = new Scene(fullFileName.get()); // #TODO Add to project file (save project option)
                    newScene->LoadScene();
                    newScene->OnLoaded();
                    Scenes::SetCurrentScene(newScene->GetSceneName());
                }
            }
            else
            {
                LOG_WARN("Drag file type unsupported: {0}", dropFileExtensionStr.get());
            }
        }
    }

}
