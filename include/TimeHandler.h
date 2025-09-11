#pragma once

#include <pico/time.h>
#include <stdint.h>

#ifndef USING_MULTICORE
#define USING_MULTICORE 1
#endif
#if USING_MULTICORE
#include <pico/multicore.h>
#endif

namespace uazips
{
    struct TimeHandler;
    void offload_time_updating(TimeHandler& th);

    struct TimeHandler
    {

        // measured in microseconds
        uint64_t TimeWhenCreated;
        uint64_t TimeThen;
        uint64_t TimeNow;
        uint64_t DeltaTime;
        //

        // measured in seconds
        float dt;

        inline TimeHandler()
        {
            TimeWhenCreated = get_absolute_time();
            TimeThen = TimeWhenCreated;
            TimeNow = TimeWhenCreated;
        }

        inline void Update()
        {
            TimeNow = get_absolute_time();   
            DeltaTime = TimeNow - TimeThen;
            TimeThen = TimeNow;
            dt = static_cast<float>(DeltaTime) / 1e6f;
        }

        inline uint64_t GetElapsed() const
        {
            return TimeNow - TimeWhenCreated;
        }
        
        inline operator uint64_t() const
        {
            return DeltaTime;
        }

        inline operator float() const
        {
            return dt;
        }

        friend void offload_time_updating(TimeHandler*);

    private:
        static TimeHandler* instance;
    };

    void offload_time_updating(TimeHandler* th)
    {
        TimeHandler::instance = th;
        multicore_launch_core1([](){
            TimeHandler::instance->Update();
        });
    }

}