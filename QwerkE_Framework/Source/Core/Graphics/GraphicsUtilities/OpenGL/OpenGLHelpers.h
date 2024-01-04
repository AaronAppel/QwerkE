#pragma once

#include "glew/GL/glew.h"

namespace QwerkE {

    struct QImageFile;

    void GLCheckforErrors();
    void GLCheckforErrors(const char* file, int line);

    GLuint LoadTextureDataToOpenGL(QImageFile& fileData);
    GLuint GLLoad2DTexture(const char* filePath, bool flipVertically = 0);
    GLuint GLLoadCubeMapTexture(const char* filePath);

}
