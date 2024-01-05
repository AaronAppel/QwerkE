#pragma once

#include "QF_Event.h"
#include "../QwerkE_Framework/Source/Core/Graphics/DataTypes/Texture.h"

namespace QwerkE {

    class JobQueuedEvent : public Event
    {
    public:
        JobQueuedEvent();

        void Process();
    };

}

