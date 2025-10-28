#include <hardware/DualActionSensor.h>

namespace uazips
{

    DualActionSensor::DualActionSensor(uint8_t gpio_pin)
        : IODevice<GPIOEvent>(gpio_pin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE)
    {
    }

    const char* DualActionSensor::AddEdgeRiseListener(const char* name, const EventListener& action)
    {
        return AddListener(name, [&action](const GPIOEvent* event){
            if (event->GetEventMask() & GPIO_IRQ_EDGE_RISE)
                action(event);
        });
    }

    const char* DualActionSensor::AddEdgeFallListener(const char* name, const EventListener& action)
    {
        return AddListener(name, [&action](const GPIOEvent* event){
            if (event->GetEventMask() & GPIO_IRQ_EDGE_FALL)
                action(event);
        });
    }

    void DualActionSensor::HandleIRQ(uint32_t events)
    {
        if (events & level_mask)
        {
            Event* event = new GPIOEvent(this, gpio_pin, events);
            queue_try_add(&Event::event_queue, &event);
        }
    }

}