#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>

// #TODO Try to find a way of simplifying unregistering so external ids are not needed,
// but also catch dangling or invalid callbacks. Handles seem to be the best way, but forcing the
// caller to be responsible for the handle is sort of lame.
// It would also be great to standardize registration for free, lambda, and member function to be the same,
// for calling convenience. Avoid caller needing to know how to register different callbacks.

namespace QwerkE {

    template<typename... Args>
    class EventCallback;

    template<typename... Args>
    class EventCallbackHandle
    {
    public:
        EventCallbackHandle() = default;

        EventCallbackHandle(const EventCallbackHandle&) = delete;
        EventCallbackHandle& operator=(const EventCallbackHandle&) = delete;

        EventCallbackHandle(EventCallbackHandle&& other) noexcept :
            event(other.event), id(other.id)
        {
            other.event = nullptr;
            other.id = INVALID_ID;
        }

        EventCallbackHandle& operator=(EventCallbackHandle&& other) noexcept
        {
            if (this != &other) {
                Unregister();
                event = other.event;
                id = other.id;
                other.event = nullptr;
                other.id = INVALID_ID;
            }
            return *this;
        }

        ~EventCallbackHandle() {
            Unregister();
        }

        void Unregister() {
            if (IsRegistered()) {
                event->Unregister(id);
                event = nullptr;
                id = INVALID_ID;
            }
        }

        bool IsRegistered() const {
            return event != nullptr && id != INVALID_ID;
        }

    private:
        friend class EventCallback<Args...>;

        EventCallbackHandle(EventCallback<Args...>* evt, std::uint64_t callbackId) :
            event(evt),
            id(callbackId)
        { }

        const std::uint64_t INVALID_ID = 0;
        std::uint64_t id = INVALID_ID;
        EventCallback<Args...>* event = nullptr;
    };

    template<typename... Args>
    class EventCallback {
    public:
        using HandleType = EventCallbackHandle<Args...>;
        using CallbackType = std::function<void(Args...)>;

        [[nodiscard]] HandleType Register(CallbackType&& callback)
        {
            std::uint64_t id = nextId++;
            callbacks.emplace(id, std::forward<CallbackType>(callback));
            return HandleType(this, id);
        }

        void Invoke(Args... args)
        {
            invoking = true;
            for (const auto& [id, callback] : callbacks)
            {
                if (callback)
                    callback(args...);
            }
            invoking = false;

            for (size_t i = 0; i < idsToUnregisterDelayed.size(); i++)
            {
                Unregister(idsToUnregisterDelayed[i]);
            }
        }

        [[nodiscard]] HandleType operator+=(CallbackType&& callback)
        {
            return Register(std::forward<CallbackType>(callback));
        }

        void operator-=(HandleType& connection)
        {
            // #TODO Add to a buffer to unregister later if
            if (!invoking)
            {
                connection.Unregister();
            }
            else
            {
                idsToUnregisterDelayed.push_back(connection.id);
            }
        }

        void operator()(Args... args) const
        {
            Invoke(args...);
        }

    private:
        friend class EventCallbackHandle<Args...>;

        void operator-=(std::uint64_t id)
        {
            Unregister(id);
        }

        void Unregister(std::uint64_t id)
        {
            callbacks.erase(id);
        }

        // #TODO Consider using 2 vectors (std::uint64_t ids[], and CallbackType callbacks[]) to keep memory contiguous for faster iteration.
        // Ids only needed for unregistering
        std::unordered_map<std::uint64_t, CallbackType> callbacks;

        std::vector<std::uint64_t> idsToUnregisterDelayed;
        bool invoking = true;
        std::uint64_t nextId = 1;
    };

    template<typename... Args>
    auto MakeEventCallback()
    {
        return EventCallback<Args...>{};
    }

}
