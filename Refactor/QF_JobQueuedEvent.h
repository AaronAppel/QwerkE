#pragma once

#include "QF_Event.h"

namespace QwerkE {

    class JobQueuedEvent : public Event
    {
    public:
        JobQueuedEvent();

        void Process();
    };

}

