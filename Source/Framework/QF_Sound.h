#pragma once

// #include "Libraries/OpenAL/include/al.h"

namespace QwerkE {

    class Sound // #TODO Becomes: class Sound
    {
    public:
        Sound();
        ~Sound();

        void SetOrientation(vec3f pos, vec3f rot, vec3f vel);

        // #TODO Abstract from OpenAL
        // void Play(ALuint buffer);
        void Play();

    private:
        // #TODO Abstract from OpenAL
        // ALuint m_SourceID = 0;
    };

}
