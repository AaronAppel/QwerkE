#pragma once

#ifdef _QOPENGL
typedef unsigned int GLuint;
#endif

#include "QF_FrameBufferObject.h"

namespace QwerkE {

    void CheckAllGraphicsErrors();
    void CheckGraphicsErrors();
    void CheckGraphicsErrors(char* file, int line);
    void CheckGraphicsErrors(const char* file, int line);

    GLuint Load2DTexture(const char* filename, bool flipVertically = 0);
    GLuint LoadCubeMapTexture(const char* filename);

    // GLuint Duplicate2DTexture(GLuint handle, int w, int h, int x, int y); // #TODO Implement
    GLuint CopyFBOToTexture(FrameBufferObject& handle, int w, int h, int x = 0, int y = 0);

    // #TODO Move to a better place
    // #TODO Pick a standard (input OR attribute)
    // Shader variable prefixes

    constexpr const char* InputKeyword = "in";
    constexpr const char* OutputKeyword = "out";
    constexpr const char* UniformKeyword = "uniform";

    constexpr const char* AttributeNamePrefix = "a_";
    constexpr const char* InputPrefix = "i_";
    constexpr const char* UniformPrefix = "u_";
    constexpr const char* TransferPrefix = "t_";
    constexpr const char* VaryingPrefix = "v_";

}
