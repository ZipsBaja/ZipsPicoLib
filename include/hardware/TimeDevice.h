#pragma once

#include <ZipsLib.h>
#include <event/Event.h>

namespace uazips
{

    class TimeDevice : public EventActionSupplier
    {
    protected:
        bool is_active;

    public:
        TimeDevice(const EventHandler& action);
        TimeDevice(const BasicEventHandler& action);
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
        RepeatingTimer(const EventHandler& action);
        RepeatingTimer(const BasicEventHandler& action);
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
        CountdownTimer(const EventHandler& action);
        CountdownTimer(const BasicEventHandler& action);
        virtual ~CountdownTimer();

        virtual void Begin(int32_t ms) override;
        virtual void End() override;
    };

}