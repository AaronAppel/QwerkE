#include "QF_Texture.h"

#include <string>

#include "QF_Defines.h"
#include "QF_Assets.h"

namespace QwerkE {

    void Texture::Initialize()
    {
#ifdef OpenGL
        s_Handle = Load2DTexture(TexturesFolderPath(s_FileName.c_str()));
#else
#pragma warning "Define graphics library!"
#endif
        Assets::TextureLoaded(s_FileName.c_str(), this);
    }

}
