#pragma once

#ifdef _QOPENAL
#include "Libraries/OpenAL/al.h"
#endif

#include "QF_AudioComponent.h"

namespace QwerkE {

#ifdef _QOPENAL
    class OpenAL_AudioComponent : public AudioComponent
    {
    public:
        OpenAL_AudioComponent();

        int Play();

    private:
        ALuint m_Buffer = 0;
    };
#endif

}
