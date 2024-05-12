#pragma once

namespace QwerkE {

    class Event;

    namespace Events {

        void Initialize();
        void Shutdown();

        void QueueEvent(Event* _event);

        void ProcessEvents();

    }

}
