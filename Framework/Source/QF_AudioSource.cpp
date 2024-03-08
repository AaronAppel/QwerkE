#include "QF_AudioSource.h"

#ifdef _QOPENAL
#include "Libraries/OpenAL/al.h"
#endif

namespace QwerkE {

    void AudioSource::SetOrientation(vec3 pos, vec3 rot, vec3 vel)
    {
#ifdef _QOPENAL
        alSourcef(m_SourceID, AL_GAIN, 1.0f);
        alSourcef(m_SourceID, AL_PITCH, 1.0f);
        alSource3f(m_SourceID, AL_VELOCITY, vel.x, vel.y, vel.z);
        alSource3f(m_SourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
#endif
    }

    void AudioSource::Play(ALuint buffer)
    {
#ifdef _QOPENAL
        alSourcei(m_SourceID, AL_LOOPING, AL_FALSE);
        alSourcei(m_SourceID, AL_BUFFER, buffer);
        alSourcePlay(m_SourceID);
#endif
    }

}
