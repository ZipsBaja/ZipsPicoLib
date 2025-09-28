#pragma once

#include <functional>
#include <stdint.h>
#include <pico/stdlib.h>

namespace uazips
{

    class Switch
    {
    private:
        bool state;
        uint8_t pin;
        std::function<void()> action;

    public:
        inline Switch(uint8_t pin, const std::function<void()>& action)
            : pin(pin), action(action), state(false)
        {
        }

        inline void Poll()
        {
            state = gpio_get(pin);
            if (state)
                action();
        }

        inline void SetAction(const std::function<void()>& action)
        {
            this->action = action;
        }
    };

}