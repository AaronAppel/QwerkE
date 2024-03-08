#pragma once

#include "QF_Event.h"
#include "QF_Texture.h"

namespace QwerkE {

    class JobQueuedEvent : public Event
    {
    public:
        JobQueuedEvent();

        void Process();
    };

}

