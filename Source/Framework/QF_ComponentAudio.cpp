#include "QF_ComponentAudio.h"

#ifdef _QOPENAL
#define AL_LIBTYPE_STATIC
#include "Libraries/openal-soft/include/AL/al.h"
#endif

#include "QF_Assets.h"
#include "QF_Sound.h"

namespace QwerkE {

    ComponentAudio::ComponentAudio()
    {
        alGenSources(1, &m_SourceHandle);

        // if (m_SourceHandle) #TODO Validate handle

        ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

        // #TODO Support all source values in al.h, lines 121-486

        // alSourcei(m_SourceHandle, AL_SOURCE_RELATIVE, ALboolean);
        // alSourcef(m_SourceHandle, AL_CONE_INNER_ANGLE, ALfloat);
        // alSourcef(m_SourceHandle, AL_CONE_OUTER_ANGLE, ALfloat);

        alSourcef(m_SourceHandle, AL_PITCH, 1.0f);

        alSource3f(m_SourceHandle, AL_POSITION, 0.0f, 0.0f, 0.0f);
        alSource3f(m_SourceHandle, AL_DIRECTION, 0.0f, 1.0f, 0.0f);
        alSource3f(m_SourceHandle, AL_VELOCITY, 0.0f, 0.0f, 0.0f);

        alSourcei(m_SourceHandle, AL_LOOPING, AL_FALSE);
        // alSourcei(m_SourceHandle, AL_BUFFER, ALuint);
        alSourcef(m_SourceHandle, AL_GAIN, 1.0f);
        // alSourcef(m_SourceHandle, AL_MIN_GAIN, ALfloat); // 0-1
    }

    ComponentAudio::~ComponentAudio()
    {
        alDeleteSources(1, &m_SourceHandle);
    }

    void ComponentAudio::Initialize()
    {
        m_Sound = Assets::Get<QwerkE::Sound>(m_SoundGuid);
    }

    void ComponentAudio::SetOrientation(vec3f pos, vec3f rot, vec3f vel)
    {
        alSource3f(m_SourceHandle, AL_VELOCITY, vel.x, vel.y, vel.z);
        alSource3f(m_SourceHandle, AL_POSITION, 0.0f, 0.0f, 0.0f);
    }

    void ComponentAudio::SetSoundGuid(GUID soundGuid)
    {
        m_Sound = Assets::Get<Sound>(soundGuid);
        m_SoundGuid = m_Sound->Guid();
    }

    void ComponentAudio::Play()
    {
        if (m_Sound)
        {
            m_Sound->Play(m_SourceHandle);
        }
    }

}
