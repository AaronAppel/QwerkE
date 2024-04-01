#include "QF_JobQueuedEvent.h"

#include "QF_Assets.h"
#include "QF_Enums.h"
#include "QF_Jobs.h"

namespace QwerkE {

    JobQueuedEvent::JobQueuedEvent()
    {
        m_EventType = eEventTypes::eEvent_JobQueued;
    }

    void JobQueuedEvent::Process()
    {
        Jobs::ProcessTasks(); // #TODO Improve
    }

}
