#include "QF_AssetLoadedEvent.h"

#include "QF_Log.h"

namespace QwerkE {

    AssetLoadedEvent::AssetLoadedEvent(std::shared_ptr<QImageFile> asset) :
        m_Asset(asset)
    {
        m_EventType = eEventTypes::eEvent_AssetLoaded;
    }

    void AssetLoadedEvent::Process()
    {
        return;
        if (m_Asset->s_Data)
        {
            LOG_TRACE("{0} Processing data for {1}", __FUNCTION__, m_Asset->s_FileName.c_str());
            // Take loaded char* data and feed it to OpenGL for a handle
            // Texture* texture = new Texture();
            // texture->s_FileName = m_Asset.s_FileName;
            // texture->s_Handle = LoadTextureDataToOpenGL(m_Asset); // #TODO Remove OpenGL dependency

            // int handle = LoadTextureDataToOpenGL(*m_Asset);

            if (0) // handle > 0) // if (texture->s_Handle > 0)
            {
                // Update texture data in resource manager
                // Assets::TextureLoaded(m_Asset->s_FileName.c_str(), handle);
            }
            else
            {
                // delete texture; // #TODO Handle load error
            }
        }
        else
        {
            LOG_TRACE("{0} Invalid data for {1}", __FUNCTION__, m_Asset->s_FileName.c_str());
        }
    }

}
