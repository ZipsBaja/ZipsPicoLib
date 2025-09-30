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
            gpio_init(pin);
            gpio_set_dir(pin, GPIO_IN);
            gpio_pull_down(pin);
        }

        inline bool Poll()
        {
            state = gpio_get(pin);
            if (state)
                action();
            return state;
        }

        inline void SetAction(const std::function<void()>& action)
        {
            this->action = action;
        }
    };

}