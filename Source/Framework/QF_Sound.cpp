#include "QF_Sound.h"

// #include "Libraries/OpenAL/include/al.h"

namespace QwerkE {

    Sound::Sound()
    {
        // alGenSources(1, &m_SourceID);
    }

    Sound::~Sound()
    {
        // alDeleteSources(1, &m_SourceID);
    }

    void Sound::SetOrientation(vec3f pos, vec3f rot, vec3f vel)
    {
        // alSourcef(m_SourceID, AL_GAIN, 1.0f);
        // alSourcef(m_SourceID, AL_PITCH, 1.0f);
        // alSource3f(m_SourceID, AL_VELOCITY, vel.x, vel.y, vel.z);
        // alSource3f(m_SourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
    }

    // void Sound::Play(ALuint buffer)
    void Sound::Play()
    {
        // alSourcei(m_SourceID, AL_LOOPING, AL_FALSE);
        // alSourcei(m_SourceID, AL_BUFFER, buffer);
        // alSourcePlay(m_SourceID);
    }

}
