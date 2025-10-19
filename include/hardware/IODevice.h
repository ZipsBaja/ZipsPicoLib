#pragma once

#include <ZipsLib.h>
#include <event/Event.h>

namespace uazips
{
    template<class EventType = GPIOEvent>
    class IODevice : public IRQHandler<EventType>
    {
    protected:
        uint8_t gpio_pin;
        gpio_irq_level level;
        
        static IRQHandler<EventType>* instances[PICO_TOTAL_GPIO_PINS];
        
        static void GPIODispatch(uint8_t pin, uint32_t events)
        {
            if (instances[pin])
                instances[pin]->HandleIRQ(events);
        }

    public:
        IODevice(uint8_t gpio_pin, gpio_irq_level level, uint64_t us_debouncing = 0)
            : IRQHandler<EventType>(us_debouncing), gpio_pin(gpio_pin), level(level)
        {
            instances[gpio_pin] = this;
            gpio_init(gpio_pin);
            gpio_set_dir(gpio_pin, GPIO_IN);
            gpio_pull_down(gpio_pin);

            gpio_set_irq_enabled_with_callback(gpio_pin, level, true, (gpio_irq_callback_t)&GPIODispatch);
        }   
        virtual ~IODevice()
        {
            instances[gpio_pin] = nullptr;
            gpio_set_irq_enabled_with_callback(gpio_pin, level, false, nullptr);
        }

        virtual void Enable() override
        {
            gpio_set_irq_enabled_with_callback(gpio_pin, level, true, (gpio_irq_callback_t)&GPIODispatch);
        }

        virtual void Disable() override
        {
            gpio_set_irq_enabled_with_callback(gpio_pin, level, false, nullptr);
        }

        inline uint8_t GetPin() const
        {
            return gpio_pin;
        }
    
    };

    template<class EventType>
    IRQHandler<EventType>* IODevice<EventType>::instances[PICO_TOTAL_GPIO_PINS] = {};


}