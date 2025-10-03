#pragma once

#include <stdint.h>
#include <cstddef>
#include <functional>

#include <hardware/irq.h>
#include <pico/util/queue.h>
#include <pico/stdlib.h>

namespace uazips
{

    constexpr size_t queue_max_size = 64;

    class IRQHandler
    {
    protected:
        std::function<void()> action;

    public:
        inline IRQHandler(const std::function<void()>& action) : action(action) {}
        inline IRQHandler() : IRQHandler(nullptr) {}
        
        virtual void HandleIRQ(uint32_t events) = 0;
    };

    class Event
    {
    private:
        static bool is_queue_initialized;

    protected:
        std::function<void()> action;

    public:
        static queue_t event_queue;
    
        // If mutlicore is enabled, ideally set-up your devices in the second core and call this line to poll them.
        // @param endless_loop Begin a while (true) loop
        // @param us_debouncing Amount of microseconds to delay the event checking. This is a software solution to debouncing.
        static void HandleAllEvents(bool endless_loop, uint32_t us_debouncing = 0);

    public:
        Event(const std::function<void()>& action);
        inline Event() : Event(nullptr) {}
        
        virtual ~Event() = default;

        virtual void HandleEvent();
    };

    class GPIOEvent : public Event
    {
    protected:
        uint8_t gpio_pin;

    public:
        GPIOEvent(uint8_t gpio_pin, const std::function<void()>& action);
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
        ButtonEvent(uint8_t gpio_pin, const std::function<void()>& action);
    };

    class TimerEvent : public Event
    {
    public:
        inline TimerEvent() : Event() {}
        TimerEvent(const std::function<void()>& action);

    };

}