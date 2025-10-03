#pragma once

#include <ZipsLib.h>
#include <event/Event.h>

namespace uazips
{
    class IODevice : public IRQHandler
    {
    protected:
        uint8_t gpio_pin;
        
        static IRQHandler* instances[PICO_TOTAL_GPIO_PINS];
        
        static void GPIODispatch(uint8_t pin, uint32_t events);

    public:
        IODevice(uint8_t gpio_pin, const std::function<void()>& action);
        virtual ~IODevice();

        inline uint8_t GetPin() const
        {
            return gpio_pin;
        }
    
    };

}