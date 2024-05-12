#pragma once

#include "OpenAL/include/al.h"

#include "QF_AudioComponent.h"

namespace QwerkE {

    class OpenAL_AudioComponent : public AudioComponent
    {
    public:
        OpenAL_AudioComponent();

        int Play();

    private:
        ALuint m_Buffer = 0;
    };

}
