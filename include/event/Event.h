#pragma once

#include <util/TypeUtils.h>

#include <stdint.h>
#include <cstddef>
#include <functional>
#include <vector>

#include <hardware/irq.h>
#include <pico/util/queue.h>
#include <pico/stdlib.h>

namespace uazips
{
    class Event;

    constexpr size_t queue_max_size = 64;

    class EventSourceBase
    {
    public:
        virtual ~EventSourceBase() = default;
    };

    template<class EventType> requires Extends<EventType, Event>
    class EventSource : public EventSourceBase
    {
    public:
        using EventListener = std::function<void(const EventType*)>;

    protected:
        std::vector<EventListener> listeners;

    public:
        inline void AddListener(const EventListener& listener)
        {
            listeners.push_back(listener);
        }
        inline void RemoveListener(const EventListener& listener)
        {
            std::erase(listeners, listener);
        }

        void AlertListeners(const EventType* event)
        {
            for (auto&& l : listeners)
                l(event);
        }
    };

    template<class EventType>
    class IRQHandler : public EventSource<EventType>
    {
    public:
        inline IRQHandler() : EventSource<EventType>() {}
        virtual inline ~IRQHandler() = default;
        
        virtual void HandleIRQ(uint32_t events) = 0;
    };

    class Event
    {
    private:
        static bool is_queue_initialized;

    protected:
        EventSourceBase* source;

    public:
        static queue_t event_queue;
    
        // If mutlicore is enabled, ideally set-up your devices in the second core and call this line to poll them.
        // @param endless_loop Begin a while (true) loop
        // @param us_debouncing Amount of microseconds to delay the event checking. This is a software solution to debouncing.
        static void HandleAllEvents(bool endless_loop, uint32_t us_debouncing = 0);

    public:
        Event(EventSourceBase* source);
        virtual ~Event() = default;

        template<class EventSourceType> requires Extends<EventSourceType, EventSourceBase>
        inline EventSourceType* GetSource() const
        {
            return dynamic_cast<EventSourceType*>(source);
        }

        virtual void HandleEvent() {};
    };

    class GPIOEvent : public Event
    {
    protected:
        uint8_t gpio_pin;

    public:
        inline GPIOEvent(EventSourceBase* source, uint8_t gpio_pin) : Event(source) {};

        inline uint8_t GetPin() const
        {
            return gpio_pin;
        }
    };

    class ButtonEvent : public GPIOEvent
    {
    public:
        inline ButtonEvent(EventSourceBase* source, uint8_t gpio_pin) : GPIOEvent(source, gpio_pin) {}
    };

    class TimerEvent : public Event
    {
    public:
        inline TimerEvent(EventSourceBase* source) : Event(source) {}

    };

}