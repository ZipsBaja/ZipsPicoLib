#pragma once

#include "IODevice.h"

namespace uazips
{

    class Button : public IODevice<ButtonEvent>
    {   
    public:
        inline Button(uint8_t gpio_pin, uint64_t us_debouncing = 0)
            : IODevice<ButtonEvent>(gpio_pin, us_debouncing) {}
        virtual ~Button() = default;
        
        virtual void HandleIRQ(uint32_t events) override;
    };

}