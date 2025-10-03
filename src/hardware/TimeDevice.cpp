#include <hardware/TimeDevice.h>
#include <time.h>

namespace uazips
{

    TimeDevice::TimeDevice(const EventHandler& action)
        : is_active(false), EventActionSupplier(action)
    {
    }

    bool RepeatingTimer::TimerCallback(repeating_timer_t* timer)
    {
        if (RepeatingTimer* self = (RepeatingTimer*)timer->user_data)
        {
            static TimerEvent event;
            event = TimerEvent(self->action);
            Event* ptr = &event;
            queue_try_add(&Event::event_queue, &ptr);
        }
        return true;
    }

    RepeatingTimer::RepeatingTimer(const EventHandler& action)
        : TimeDevice(action)
    {
    }

    RepeatingTimer::RepeatingTimer(const BasicEventHandler& action)
        : TimeDevice(action)
    {
    }

    RepeatingTimer::~RepeatingTimer()
    {
        End();
    }

    void RepeatingTimer::Begin(int32_t ms)
    {
        if (is_active)
            return;
        is_active = add_repeating_timer_ms(ms, (repeating_timer_callback_t)&TimerCallback, this, &repeat_timer);
    }

    void RepeatingTimer::End()
    {
        if (is_active)
        {
            cancel_repeating_timer(&repeat_timer);
            is_active = false;
        }
    }

    bool CountdownTimer::TimerCallback(alarm_id_t id, void* timer)
    {
        if (CountdownTimer* self = (CountdownTimer*)timer)
        {
            Event* event = new TimerEvent(self->action);
            queue_try_add(&Event::event_queue, &event);
        }
        return false;
    }

    CountdownTimer::CountdownTimer(const EventHandler& action)
        : TimeDevice(action)
    {
    }
    
    CountdownTimer::CountdownTimer(const BasicEventHandler& action)
        : TimeDevice(action)
    {
    }

    CountdownTimer::~CountdownTimer()
    {
        End();
    }

    void CountdownTimer::Begin(int32_t ms)
    {
        if (id >= 0)
            return;
        id = add_alarm_in_ms(ms, (alarm_callback_t)&TimerCallback, this, false);
    }

    void CountdownTimer::End()
    {
        if (id >= 0)
        {
            cancel_alarm(id);
            id = -1;
        }
    }

}