#pragma once

#include "IODevice.h"

namespace uazips
{

    class Button : public IODevice
    {   
    public:
        Button(uint8_t gpio_pin, const EventHandler& action);
        Button(uint8_t gpio_pin, const BasicEventHandler& action);
        virtual ~Button() = default;
        
        virtual void HandleIRQ(uint32_t events) override;
    };

}