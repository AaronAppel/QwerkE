#pragma once

#ifdef _QOPENAL
#include "Libraries/OpenAL/al.h"
#else
#error "Define audio library!"
#endif

namespace QwerkE {

    class AudioSource
    {
    public:
        AudioSource() { alGenSources(1, &m_SourceID); }
        ~AudioSource() { alDeleteSources(1, &m_SourceID); }

        void SetOrientation(vec3 pos, vec3 rot, vec3 vel);

        void Play(ALuint buffer); // #TODO Abstract from OpenAL

    private:
        ALuint m_SourceID = 0; // #TODO Abstract from OpenAL
    };

}
