#include "QF_EventManager.h"

#include "Libraries/pThreads/pthread.h"

#include "QF_Event.h"
#include "QF_Log.h"
#include "QF_Resources.h"

namespace QwerkE
{

    // #TODO Review QueueEvent() thread safety
    static pthread_mutex_t* s_mutex = nullptr;
    const int EventManager::m_EventMax = 100;
    std::queue<Event*> EventManager::m_EventQueue;

    void EventManager::Initialize()
    {
        s_mutex = new pthread_mutex_t();
        *s_mutex = PTHREAD_MUTEX_INITIALIZER;
    }

    void EventManager::Shutdown()
    {
        pthread_mutex_lock(s_mutex);
        for (size_t i = 0; i < m_EventQueue.size(); i++)
        {
            if (Event* event = m_EventQueue.front())
            {
                delete event;
                m_EventQueue.pop();
            }
        }
        pthread_mutex_unlock(s_mutex);
        delete s_mutex;
    }

    void EventManager::QueueEvent(Event* _event)
    {
        pthread_mutex_lock(s_mutex);
        // #TODO Implement thread safe API for multi threaded event queuing
        if (m_EventQueue.size() < m_EventMax)
        {
            _event->SetID(Resources::CreateGUID());
            m_EventQueue.push(_event);
            LOG_INFO("Event {0} Queued!", _event->GetID());
        }
        else
        {
            LOG_ERROR("Event list is full!");
        }
        pthread_mutex_unlock(s_mutex);
    }

    void EventManager::ProcessEvents()
    {
        pthread_mutex_lock(s_mutex);
        int size = (int)m_EventQueue.size(); // #TODO Track threads and limit number using ConfigData

        for (int i = 0; i < size; i++)
        {
            Event* _event = m_EventQueue.front();
            _event->Process();

            switch (_event->GetType())
            {
            case eEventTypes::eEvent_AssetLoaded:
                LOG_TRACE("Asset loaded");
                break;
            case eEventTypes::eEvent_JobQueued:
                break;
            case eEventTypes::eEvent_InputEvent:
                break;
            case eEventTypes::eEvent_Invalid:
                LOG_ERROR("Invalid event type found!");
                break;
            default:
                LOG_ERROR("Error reading event type. Make sure all types are handled!");
                break;
            }

            m_EventQueue.pop();
        }
        pthread_mutex_unlock(s_mutex);
    }

}
