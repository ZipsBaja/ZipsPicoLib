#pragma once

#include <ZipsLib.h>
#include <event/Event.h>

namespace uazips
{

    class TimeDevice : public EventSource<TimerEvent>
    {
    protected:
        bool is_active;
        uint64_t us_elapsed;
        uint64_t us_start;
        uint64_t us_begintime;
    public:
        inline TimeDevice()
            : EventSource<TimerEvent>(), is_active(false), us_elapsed(0), us_start(0), us_begintime(0) {}
        virtual ~TimeDevice() = default;

        virtual void Begin(uint64_t ms) = 0;
        virtual void End() = 0;

        inline uint64_t GetTimeElapsedMicroseconds() const
        {
            return us_elapsed;
        }
        inline uint64_t GetTimeElapsedMilliseconds() const
        {
            return us_elapsed / (uint64_t)1000;
        }
        inline float GetTimeElapsedSeconds() const
        {
            return us_elapsed * 1e-6f;
        }

        inline uint64_t GetTimeRemainingMicroseconds() const
        {
            return us_begintime - us_elapsed;
        }
        inline uint64_t GetTimeRemainingMilliseconds() const
        {
            return GetTimeRemainingMicroseconds() / (uint64_t)1000;
        }
        inline float GetTimeRemainingSeconds() const
        {
            return GetTimeElapsedMicroseconds() * 1e-6f;
        }

        inline void UpdateTime()
        {
            us_elapsed = to_us_since_boot(get_absolute_time()) - us_start;
        }
    };

    class RepeatingTimer : public TimeDevice
    {
    private:
        repeating_timer_t repeat_timer;

        static bool TimerCallback(repeating_timer_t* timer);

    public:
        inline RepeatingTimer() : TimeDevice() {}
        virtual ~RepeatingTimer();

        virtual void Begin(uint64_t ms) override;
        virtual void End() override;
    };

    class CountdownTimer : public TimeDevice
    {
    private:
        int id;

        static bool TimerCallback(alarm_id_t id, void* timer);

    public:
        inline CountdownTimer() : TimeDevice(), id(-1) {}
        virtual ~CountdownTimer();

        virtual void Begin(uint64_t ms) override;
        virtual void End() override;
    };

}