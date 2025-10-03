#pragma once

#include <util/TypeUtils.h>

#include <stdint.h>
#include <cstddef>
#include <functional>

#include <hardware/irq.h>
#include <pico/util/queue.h>
#include <pico/stdlib.h>

namespace uazips
{
    class Event;

    // Action function for all events. An input of type Event* referencing
    // the event that is triggered, and a return of bool for the potential
    // of loop-breakouts.
    using EventHandler = std::function<bool(Event*)>;

    // For simple actions without needing to specify a return statement.
    // Will wrap into a bool function, will always return true.
    using BasicEventHandler = std::function<void(Event*)>;

    constexpr size_t queue_max_size = 64;

    class EventActionSupplier
    {
    protected:
        EventHandler action;

    public:
        EventActionSupplier(const EventHandler& action);
        EventActionSupplier(const BasicEventHandler& action);
        
        inline EventHandler GetAction() const
        {
            return action;
        }
    };

    class IRQHandler : public EventActionSupplier
    {
    public:
        inline IRQHandler(const EventHandler& action) : EventActionSupplier(action) {}
        inline IRQHandler(const BasicEventHandler& action) : EventActionSupplier(action) {}
        inline IRQHandler() : IRQHandler([](Event*){}) {}
        virtual inline ~IRQHandler() = default;
        
        virtual void HandleIRQ(uint32_t events) = 0;
    };

    class Event : public EventActionSupplier
    {
    private:
        static bool is_queue_initialized;

    public:
        static queue_t event_queue;
    
        // If mutlicore is enabled, ideally set-up your devices in the second core and call this line to poll them.
        // @param endless_loop Begin a while (true) loop
        // @param us_debouncing Amount of microseconds to delay the event checking. This is a software solution to debouncing.
        static void HandleAllEvents(bool endless_loop, uint32_t us_debouncing = 0);

    public:
        Event(const EventHandler& action);
        inline Event() : Event(nullptr) {}
        
        virtual ~Event() = default;

        virtual void HandleEvent();
    };

    class GPIOEvent : public Event
    {
    protected:
        uint8_t gpio_pin;

    public:
        GPIOEvent(uint8_t gpio_pin, const EventHandler& action);
        inline GPIOEvent() : Event() {};

        inline uint8_t GetPin() const
        {
            return gpio_pin;
        }
    };

    class ButtonEvent : public GPIOEvent
    {
    public:
        inline ButtonEvent() : GPIOEvent() {}
        ButtonEvent(uint8_t gpio_pin, const EventHandler& action);
    };

    class TimerEvent : public Event
    {
    public:
        inline TimerEvent() : Event() {}
        TimerEvent(const EventHandler& action);

    };

}