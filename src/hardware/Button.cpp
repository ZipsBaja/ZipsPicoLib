#include <hardware/Button.h>

namespace uazips
{

    Button::Button(uint8_t gpio_pin, const EventHandler& action)
        : IODevice(gpio_pin, action)
    {
    }

    Button::Button(uint8_t gpio_pin, const BasicEventHandler& action)
    :   IODevice(gpio_pin, action)
    {
    }

    void Button::HandleIRQ(uint32_t events)
    {
        if (events & GPIO_IRQ_EDGE_RISE)
        {
            Event *event = new ButtonEvent(gpio_pin, action);
            queue_try_add(&Event::event_queue, &event);
        }
    }

}