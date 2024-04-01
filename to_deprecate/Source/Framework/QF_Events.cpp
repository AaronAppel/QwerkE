#include "QF_Events.h"

#include <queue>

#include "Libraries/pThreads/pthread.h"

#include "QF_Assets.h"
#include "QF_Debug.h"
#include "QF_Event.h"
#include "QF_JobQueuedEvent.h"
#include "QF_Jobs.h"
#include "QF_Log.h"

namespace QwerkE {

    namespace Events {

        static pthread_mutex_t s_mutex;
        static const u8 s_EventMax = 50;
        static std::queue<Event*> s_EventQueue;

        void Initialize()
        {
            const u8 result = pthread_mutex_init(&s_mutex, NULL);
            ASSERT(result == 0, "Error initializing pthread mutex!");
        }

        void Shutdown()
        {
            pthread_mutex_lock(&s_mutex); // #TODO Review using pthread_mutex_trylock instead
            while (!s_EventQueue.empty())
            {
                if (Event* event = s_EventQueue.front())
                {
                    delete event;
                }
                s_EventQueue.pop();
            }
            pthread_mutex_unlock(&s_mutex);
            pthread_mutex_destroy(&s_mutex);
        }

        void QueueEvent(Event* _event)
        {
            pthread_mutex_lock(&s_mutex);
            // #TODO Implement thread safe API for multi threaded event queuing
            if (s_EventQueue.size() < s_EventMax)
            {
                _event->SetID(Assets::GetGUID());
                s_EventQueue.push(_event);
                LOG_INFO("Event {0} Queued of type {1}", _event->GetID(), (int)_event->GetType());
            }
            else
            {
                LOG_ERROR("Event list is full!");
                delete _event; // #TODO Job memory leaked
            }
            pthread_mutex_unlock(&s_mutex);
        }

        void ProcessEvents()
        {
            pthread_mutex_lock(&s_mutex);
            int size = (int)s_EventQueue.size(); // #TODO Track threads and limit number using ConfigData

            for (int i = 0; i < size; i++)
            {
                Event* event = s_EventQueue.front();
                event->Process();

                switch (event->GetType())
                {
                case eEventTypes::eEvent_AssetLoaded:
                    break;

                case eEventTypes::eEvent_JobQueued:
                    break;

                case eEventTypes::eEvent_InputEvent:
                    break;

                case eEventTypes::eEvent_Invalid:
                default:
                    LOG_ERROR("Unsupported event type found!");
                    break;
                }

                s_EventQueue.pop();
                delete event;
            }
            pthread_mutex_unlock(&s_mutex);
        }

        // #FEATURE F0005
        // template <typename T>
        // void DelayedDeletion(sPtr<T> objectToDeleteLater, float timeUntilDeletion) // #EXPERIMENTAL
        // {
        //     // #TODO Add timed event system
        //     objectToDeleteLater.reset();
        // }

    }

}
