#pragma once

#include <pico/stdlib.h>
#include <cstdint>
#include <cstddef>

struct PinMode
{
    uint8_t pin;
    bool mode;
};

class VirtualMultiplexer
{
private:
    PinMode* pins_used;
    const size_t total_pins;

public:
    /*
    * Constructs a new mutliplexer object.
    * @param pins All of the pins being used by this multiplexer.
    * @param total_pins The number of pins in the previous parameter's array. Use `countof(pins)` or `sizeof(pins)/sizeof(uint8_t)` if the array is stack-allocated.
    * @param dir The GPIO direction for the pins.
    */
    VirtualMultiplexer(uint8_t pins[], size_t total_pins, bool dir = 0);

    /*
    * Delete heap-allocated data.
    */
    ~VirtualMultiplexer();

    // Sets a specific pin number to high and all others to low.
    inline void SetHigh(uint8_t pin);

    // Sets a pin number to high by its index and all others to low, in the pin array entered during object construction.
    inline uint8_t SetHighByIndex(size_t index);

    // erm... freaking epic alert???
    inline uint8_t GetHigh() const;

    inline size_t GetTotalPins() const
    {
        return total_pins;
    }

    inline uint8_t GetPinByIndex(size_t index) const
    {
        if (index >= total_pins)
            return pins_used[total_pins - 1].pin;
        return pins_used[index].pin;
    }
};