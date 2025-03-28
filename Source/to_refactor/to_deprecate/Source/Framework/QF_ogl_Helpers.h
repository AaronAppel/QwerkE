#pragma once

#include "Libraries/glew/glew.h"

#include "QF_FileSystem.h"

namespace QwerkE {

    void GLCheckforErrors();
    void GLCheckforErrors(const char* file, int line);

    GLuint LoadTextureDataToOpenGL(QImageFile& fileData);
    GLuint GLLoad2DTexture(const char* filePath, bool flipVertically = 0);
    GLuint GLLoadCubeMapTexture(const char* filePath);

}
