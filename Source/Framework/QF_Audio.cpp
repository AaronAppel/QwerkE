#include "QF_Audio.h"

#include "QF_LibrariesLoad.h"

#include "QF_Log.h"
#include "QC_Vector.h"

#ifdef OpenAL
#include "QF_oal_AudioManager.h"
#endif

namespace QwerkE {

#ifdef OpenAL
    OpenALAudioManager* Audio::m_AudioManager = nullptr;
#endif
    bool Audio::m_Initialized = false;

    bool Audio::Initialize()
    {
#ifdef OpenAL
        m_AudioManager = new OpenALAudioManager();
#endif
        m_Initialized = m_AudioManager->Initialize();
        return m_Initialized;
    }

    void Audio::Shutdown()
    {
        delete m_AudioManager;
        m_Initialized = false;
    }

    void Audio::PlaySound(const char* name)
    {
        // #TODO Could be a case for DEBUG_ONLY_CHECK() to catch uninitialized calls without need for if checking?
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
        {
            m_AudioManager->SetListenerOrientation(position, velocity);
        }
        else
        {
            // #TODO vec3 expansion helper/macro
            LOG_TRACE("Audio.SetListenerOrientation(({0},{1},{2}), ({3,4,5}))", position.x, position.y, position.z, velocity.x, velocity.y, velocity.z);
        }
    }

}
