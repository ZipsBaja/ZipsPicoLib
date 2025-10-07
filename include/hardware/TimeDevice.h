#pragma once

#include <ZipsLib.h>
#include <event/Event.h>

namespace uazips
{

    class TimeDevice : public EventSource<TimerEvent>
    {
    protected:
        bool is_active;

    public:
        inline TimeDevice() : EventSource<TimerEvent>(), is_active(false) {}
        virtual ~TimeDevice() = default;

        virtual void Begin(int32_t ms) = 0;
        virtual void End() = 0;
    };

    class RepeatingTimer : public TimeDevice
    {
    private:
        repeating_timer_t repeat_timer;

        static bool TimerCallback(repeating_timer_t* timer);

    public:
        inline RepeatingTimer() : TimeDevice() {}
        virtual ~RepeatingTimer();

        virtual void Begin(int32_t ms) override;
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

        virtual void Begin(int32_t ms) override;
        virtual void End() override;
    };

}