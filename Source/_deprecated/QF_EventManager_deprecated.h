#pragma once

namespace QwerkE {

    class Event;

    namespace Events {

        static void Initialize();
        static void Shutdown();

        static void QueueEvent(Event* _event);

        static void ProcessEvents();

        static const int m_EventMax;
        static std::queue<Event*> m_EventQueue;

    }

}
