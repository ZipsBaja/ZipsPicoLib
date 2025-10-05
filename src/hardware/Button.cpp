#include <hardware/Button.h>

namespace uazips
{

    void Button::HandleIRQ(uint32_t events)
    {
        if (events & GPIO_IRQ_EDGE_RISE)
        {
            Event* event = new ButtonEvent(this, gpio_pin);
            queue_try_add(&Event::event_queue, &event);
        }
    }

}