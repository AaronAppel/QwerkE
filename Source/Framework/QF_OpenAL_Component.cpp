#include "QF_OpenAL_Component.h"

#include "QF_Defines.h"
#include "QF_Assets.h"

namespace QwerkE {

    OpenAL_AudioComponent::OpenAL_AudioComponent()
        : m_Buffer(Assets::GetSound(null_sound))
    {
    }

    int OpenAL_AudioComponent::Play()
    {
        alSourcePlay(m_Buffer);
        return 1;
    }

}
