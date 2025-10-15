#include <hardware/Button.h>

namespace uazips
{

    void Button::HandleIRQ(uint32_t events)
    {
        if (events & GPIO_IRQ_EDGE_RISE)
        {
            absolute_time_t now_us = get_absolute_time();
            if (absolute_time_diff_us(last_time_us, now_us) >= us_debouncing)
            {
                Event* event = new ButtonEvent(this, gpio_pin);
                queue_try_add(&Event::event_queue, &event);
                last_time_us = now_us;
            }
        }
    }

}