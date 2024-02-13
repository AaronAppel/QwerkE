#pragma once

#include <string>

#include "QF_Defines.h"
#include "QF_Resources.h"

namespace QwerkE {

    void Texture::Initialize()
    {
#ifdef OpenGL
        s_Handle = Load2DTexture(TexturesFolderPath(s_FileName.c_str()));
#else
#pragma warning "Define graphics library!"
#endif
        Resources::AddTexture(s_FileName.c_str(), this);
    }

}
