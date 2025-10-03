#include <hardware/IODevice.h>

namespace uazips
{
    IRQHandler* IODevice::instances[PICO_TOTAL_GPIO_PINS] = {};

    void IODevice::GPIODispatch(uint8_t pin, uint32_t events)
    {
        if (instances[pin])
            instances[pin]->HandleIRQ(events);
    }

    IODevice::IODevice(uint8_t gpio_pin, const std::function<void()>& action)
        : IRQHandler(action)
    {
        instances[gpio_pin] = this;
        gpio_init(gpio_pin);
        gpio_set_dir(gpio_pin, GPIO_IN);
        gpio_pull_down(gpio_pin);

        gpio_set_irq_enabled_with_callback(gpio_pin, GPIO_IRQ_EDGE_RISE, true, (gpio_irq_callback_t)&GPIODispatch);

    }

    IODevice::~IODevice()
    {
        instances[gpio_pin] = nullptr;
    }

}