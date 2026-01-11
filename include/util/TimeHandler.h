#pragma once

#include <pico/time.h>
#include <stdint.h>

struct TimeHandler
{

     // measured in microseconds
    uint64_t time_when_created;
    uint64_t time_then;
    uint64_t time_now;
    uint64_t delta_us;
    //

     // measured in seconds
    float dt;

    TimeHandler();

    void Update();

    inline uint64_t GetElapsed() const
    {
        return time_now - time_when_created;
    }
    
    inline operator uint64_t() const
    {
        return delta_us;
    }

    inline operator float() const
    {
        return dt;
    }
};