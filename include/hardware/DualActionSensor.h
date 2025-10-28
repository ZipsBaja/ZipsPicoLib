#pragma once

#include "IODevice.h"

namespace uazips
{

    class DualActionSensor : IODevice<GPIOEvent>
    {
    public:
        DualActionSensor(uint8_t gpio_pin);

        const char* AddEdgeRiseListener(const char* name, const EventListener& action);
        const char* AddEdgeFallListener(const char* name, const EventListener& action);

        virtual void HandleIRQ(uint32_t events) override;
    };

}