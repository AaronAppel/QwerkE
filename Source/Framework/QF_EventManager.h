#pragma once

#include <queue>

namespace QwerkE {

    class Event;

    class EventManager
    {
    public:
        static void Initialize();
        static void Shutdown();

        static void QueueEvent(Event* _event);

        static void ProcessEvents();

        // template <typename T>
        // static void DelayedDeletion(sPtr<T> objectToDeleteLater, float timeUntilDeletion);// #EXPERIMENTAL

    private:
        EventManager() = default;
        ~EventManager() = default;

        static const int m_EventMax;
        static std::queue<Event*> m_EventQueue;
    };

}
