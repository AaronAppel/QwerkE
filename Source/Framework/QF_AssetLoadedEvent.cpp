#include "QF_AssetLoadedEvent.h"

#include "QF_Assets.h"
#include "QF_ogl_Helpers.h"

namespace QwerkE {

    AssetLoadedEvent::AssetLoadedEvent(std::shared_ptr<QImageFile> asset) :
        m_Asset(asset)
    {
        m_EventType = eEventTypes::eEvent_AssetLoaded;
    }

    void AssetLoadedEvent::Process()
    {
        if (m_Asset->s_Data != nullptr)
        {
            // Take loaded char* data and feed it to OpenGL for a handle
            // Texture* texture = new Texture();
            // texture->s_FileName = m_Asset.s_FileName;
            // texture->s_Handle = LoadTextureDataToOpenGL(m_Asset); // #TODO Remove OpenGL dependency

            int handle = LoadTextureDataToOpenGL(*m_Asset);

            if (handle > 0) // if (texture->s_Handle > 0)
            {
                // Update texture data in resource manager
                Assets::UpdateTexture(m_Asset->s_FileName.c_str(), handle);
            }
            else
            {
                // delete texture; // #TODO Handle load error
            }
        }
    }

}
