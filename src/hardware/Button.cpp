#include <hardware/Button.h>

namespace uazips
{

    Button::Button(uint8_t gpio_pin, const std::function<void()>& action)
        : IODevice(gpio_pin, action)
    {
    }

    void Button::HandleIRQ(uint32_t events)
    {
        if (events & GPIO_IRQ_EDGE_RISE)
        {
            static ButtonEvent event;
            event = ButtonEvent(gpio_pin, action);
            Event* ptr = &event;
            queue_try_add(&Event::event_queue, &ptr);
        }
    }

}