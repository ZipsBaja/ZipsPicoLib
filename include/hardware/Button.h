#pragma once

#include <functional>
#include <stdint.h>
#include <pico/stdlib.h>

namespace uazips
{

    class Button
    {
    private:
        bool allow;
        bool state;
        uint8_t pin;
        std::function<void()> action;

    public:
        inline Button(uint8_t button_pin, const std::function<void()>& action)
            : pin(button_pin), action(action), allow(true), state(false)
        {
        }

        inline void Poll()
        {
            state = gpio_get(pin);
            if (state && allow)
            {
                allow = false;
                action();
            }
            else if (!state)
                allow = true;
        }

        inline bool IsPressed() const
        {
            return state;
        }

        inline void SetAction(const std::function<void()>& action)
        {
            this->action = action;
        }
    };

}