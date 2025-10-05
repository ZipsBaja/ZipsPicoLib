#pragma once

#include "IODevice.h"

namespace uazips
{

    class Button : public IODevice<ButtonEvent>
    {   
    public:
        inline Button(uint8_t gpio_pin) : IODevice<ButtonEvent>(gpio_pin) {}
        virtual ~Button() = default;
        
        virtual void HandleIRQ(uint32_t events) override;
    };

}