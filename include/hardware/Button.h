#pragma once

#include "IODevice.h"

namespace uazips
{

    class Button : public IODevice
    {   
    public:
        Button(uint8_t gpio_pin, const std::function<void()>& action);
        
        virtual void HandleIRQ(uint32_t events) override;
    };

}