#include <hardware/TimeDevice.h>
#include <pico/time.h>

namespace uazips
{

    void TimeDevice::OnRemoveListener()
    {
        End();
    }

    bool RepeatingTimer::TimerCallback(repeating_timer_t* timer)
    {
        if (RepeatingTimer* self = (RepeatingTimer*)timer->user_data)
        {
            Event* event = new TimerEvent(self);
            queue_try_add(&Event::event_queue, &event);
            self->us_start = to_us_since_boot(get_absolute_time());
        }
        return true;
    }

    RepeatingTimer::~RepeatingTimer()
    {
        End();
    }

    void RepeatingTimer::Begin(uint64_t ms)
    {
        if (is_active)
        {
            End();
            Begin(ms);
        }
            
        us_begintime = ms * (uint64_t)1000;
        us_start = to_us_since_boot(get_absolute_time());
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
            Event* event = new TimerEvent(self);
            queue_try_add(&Event::event_queue, &event);
            self->us_start = 0;
            self->End();
        }
        return false;
    }

    CountdownTimer::~CountdownTimer()
    {
        End();
    }

    void CountdownTimer::Begin(uint64_t ms)
    {
        if ((id >= 0) || is_active)
        {
            End();
            Begin(ms);
        }
            

        us_begintime = ms * (uint64_t)1000;
        us_start = to_us_since_boot(get_absolute_time());
        id = add_alarm_in_ms(ms, (alarm_callback_t)&TimerCallback, this, false);
        is_active = true;
    }

    void CountdownTimer::End()
    {
        if (id >= 0 || is_active)
        {
            cancel_alarm(id);
            id = -1;
            is_active = false;
        }
    }

}