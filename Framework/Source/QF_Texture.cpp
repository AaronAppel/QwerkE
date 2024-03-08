#include "QF_Texture.h"

#include <string>

#include "QF_Assets.h"
// #include "QF_Defines.h"
#include "QF_GraphicsHelpers.h"

namespace QwerkE {

    void Texture::Initialize()
    {
#ifdef _QOPENGL
        if (!Assets::TextureExists(s_FileName.c_str()))
        {
            // s_Handle = Load2DTexture(TexturesFolderPath(s_FileName.c_str()));
        }
        else
        {
            s_Handle = Assets::GetTexture(s_FileName.c_str())->s_Handle;
        }
#else
#pragma warning "Define graphics library!"
#endif
        Assets::TextureLoaded(s_FileName.c_str(), this);
    }

}
