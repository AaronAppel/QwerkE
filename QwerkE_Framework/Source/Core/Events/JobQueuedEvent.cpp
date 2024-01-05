#include "JobQueuedEvent.h"

#include "QF_Resources.h"
#include "QF_QwerkE_Enums.h"
#include "../Jobs/Jobs.h"

namespace QwerkE {

    JobQueuedEvent::JobQueuedEvent()
    {
        m_EventType = eEventTypes::eEvent_JobQueued;
    }

    void JobQueuedEvent::Process()
    {
        Jobs::ProcessTasks();
    }

}
