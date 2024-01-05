#include "QF_InputEvent.h"

namespace QwerkE {

    InputEvent::InputEvent(int value)
    {
        m_Value = value;
        m_EventType = eEventTypes::eEvent_InputEvent;
    }

}
