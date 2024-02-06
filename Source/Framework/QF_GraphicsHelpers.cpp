#include "QF_GraphicsHelpers.h"

#include <map>
#include <string>

#include "QC_StringHelpers.h"

#include "QF_Graphics_Header.h"
#include "QF_ogl_Helpers.h"
#include "QF_RenderComponent.h"
#include "QF_Resources.h"
#include "QF_Material.h"
#include "QF_MaterialComponent.h"
#include "QF_ShaderProgram.h"
#include "QF_ShaderComponent.h"
#include "QF_Renderable.h"
#include "QF_Texture.h"
#include "QF_Enums.h"
#include "QF_Defines.h"

namespace QwerkE {

    // #TODO Use #define to strip graphics checks from release builds

    void CheckAllGraphicsErrors()
    {
        // #TODO Handle all sorts of graphics system errors
    }

    #ifdef OpenGL
    #include "QF_ogl_Helpers.h"
    void CheckGraphicsErrors()
    {
        GLCheckforErrors();
    }

    void CheckGraphicsErrors(char* file, int line)
    {
        GLCheckforErrors(file, line);
    }

    void CheckGraphicsErrors(const char* file, int line)
    {
        GLCheckforErrors(file, line);
    }

    // #TODO Return a valid handle and try to avoid a stack frame if not necessary
    GLuint Load2DTexture(const char* filename, bool flipVertically)
    {
        return GLLoad2DTexture(filename, flipVertically);
    }

    GLuint LoadCubeMapTexture(const char* filename)
    {
        return GLLoadCubeMapTexture(filename);
    }

    GLuint CopyFBOToTexture(FrameBufferObject& fbo, int w, int h, int x, int y)
    {
        // #TODO abstract from library specific implementation
        // #NOTE You can specify what region of the frame buffer to take which
        // means you can easily capture a specified square from the scene.

        fbo.Bind();
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, w, h, 0);

        // Create return texture
        GLuint result;
        glGenTextures(1, &result);
        glBindTexture(GL_TEXTURE_2D, result);

        // Copy data
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, w, h, 1);
        CheckGraphicsErrors(__FILE__, __LINE__);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // , , GL_CLAMP
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        fbo.UnBind();
        glBindTexture(GL_TEXTURE_2D, 0);
        return result;
    }

    #else
    #pragma error "Define graphics library!"

    #endif

}
