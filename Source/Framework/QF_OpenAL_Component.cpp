#include "QF_OpenAL_Component.h"

#include "QF_Resources.h"
#include "QF_QwerkE_Defines.h"

namespace QwerkE {

    OpenAL_AudioComponent::OpenAL_AudioComponent()
        : m_Buffer(Resources::GetSound(null_sound))
    {
    }

    int OpenAL_AudioComponent::Play()
    {
        alSourcePlay(m_Buffer);
        return 1;
    }

}
