#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#include "QC_Guid.h"

typedef unsigned int ALuint; // From OpenAL include/AL/al.h

namespace QwerkE {

    class Sound;

    // #TODO Either make an audio listener (mic?) or source (speaker?)
    class ComponentAudio
    {
    public:
        ComponentAudio();
        ~ComponentAudio();

        void Initialize();

        void SetOrientation(vec3f pos, vec3f rot, vec3f vel);
        void SetSoundGuid(GUID soundGuid);

        const GUID& GetSoundGuid() { return m_SoundGuid; }

        void Play();

    private:
        MIRROR_PRIVATE_MEMBERS

        Sound* m_Sound = nullptr;

        GUID m_SoundGuid = GUID::Invalid;
        ALuint m_SourceHandle = 0; // #TODO Abstract OpenAL
    };

}
