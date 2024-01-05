#include "QF_Audio.h"

#include "QF_Log.h"
#include "QF_oal_AudioManager.h"
#include "QC_Vector.h"

namespace QwerkE {

    OpenALAudioManager* Audio::m_AudioManager = nullptr;
    bool Audio::m_Initialized = false;

    bool Audio::Initialize()
    {
        m_AudioManager = new OpenALAudioManager();
        m_Initialized = m_AudioManager->Initialize();

        // CheckForOpenALErrors()

        return m_Initialized;
    }

    void Audio::Shutdown()
    {
        m_Initialized = false;
        delete m_AudioManager;
    }

    void Audio::PlaySound(const char* name)
    {
        if (m_Initialized)
        {
            m_AudioManager->PlaySound(name);
        }
        else
        {
            LOG_TRACE("Audio.PlaySound()");
        }
    }

    void Audio::SetListenerOrientation(vec3 position, vec3 velocity)
    {
        if (m_Initialized)
            m_AudioManager->SetListenerOrientation(position, velocity);
        else
            LOG_TRACE("Audio.SetListenerOrientation()");
    }

}
