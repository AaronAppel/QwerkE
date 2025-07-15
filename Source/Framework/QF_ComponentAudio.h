#pragma once

// #include "OpenAL/include/al.h"

namespace QwerkE {

    class ComponentAudio
    {
    public:
        // ComponentAudio() { alGenSources(1, &m_SourceID); }
        // ~ComponentAudio() { alDeleteSources(1, &m_SourceID); }

        void SetOrientation(vec3f pos, vec3f rot, vec3f vel);

        // void Play(ALuint buffer);
        // int Play(ALuint buffer)
        // {
        //     // alSourcePlay(m_Buffer);
        //     if (m_Sound) m_Sound->Play();
        //     return 1;
        // }

    private:
        // Sound* m_Sound = nullptr;
        // ALuint m_SourceID = 0;
        u32 m_Placeholder = 0;
    };

}
