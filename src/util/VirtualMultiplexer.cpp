#include <util/VirtualMultiplexer.h>

VirtualMultiplexer::VirtualMultiplexer(uint8_t pins[], size_t total_pins, bool dir)
    : total_pins(total_pins)
{
    pins_used = new PinMode[total_pins];
    for (int i = 0; i < total_pins; i++)
    {
        pins_used[i] = {pins[i], 0};
        gpio_init(pins_used[i].pin);
        gpio_set_dir(pins[i], dir);
        gpio_put(pins[i], 0);
    }
    pins_used[0].mode = 1;
    gpio_put(pins[0], 1);
}

VirtualMultiplexer::~VirtualMultiplexer()
{
    delete[] pins_used;
}

void VirtualMultiplexer::SetHigh(uint8_t pin)
{
    for (int i = 0; i < total_pins; i++)
        if (pins_used[i].pin == pin)
        {
            pins_used[i].mode = 1;
            gpio_put(pin, 1);
        }
        else
        {
            pins_used[i].mode = 0;
            gpio_put(pin, 0);
        }
}

uint8_t VirtualMultiplexer::SetHighByIndex(size_t index)
{
    uint8_t high_pin = 0;
    for (int i = 0; i < total_pins; i++)
    {
        if (index == i)
        {
            pins_used[i].mode = 1;
            high_pin = pins_used[i].pin;
            gpio_put(high_pin, 1);
        }
        else
        {
            pins_used[i].mode = 0;
            gpio_put(pins_used[i].pin, 0);
        }
    }
    return high_pin;
}

uint8_t VirtualMultiplexer::GetHigh() const
{
    for (int i = 0; i < total_pins; i++)
    {
        if (pins_used[i].mode = 1)
            return pins_used[i].pin;
    }
    return 0;
}