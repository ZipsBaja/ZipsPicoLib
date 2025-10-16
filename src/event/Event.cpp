#include <event/Event.h>
#include <ZipsLib.h>

#if USING_MULTICORE
#include <pico/multicore.h>
#endif

namespace uazips
{
#if USING_MULTICORE
    static void begin_core_1()
    {
        Event::HandleAllEvents(true);
    }
#endif

    bool Event::is_queue_initialized = false;
    queue_t Event::event_queue = {};

    void Event::HandleAllEvents(bool endless_loop)
    {
        if (endless_loop)
        {
            while (1)
            {
                Event* event;
                queue_remove_blocking(&event_queue, &event);
                if (event)
                {
                    event->HandleEvent();
                    event->source->Dispatch(event);
                    delete event;
                }
            }
        }
        else
        {
            Event* event;
            queue_remove_blocking(&event_queue, &event);
            if (event)
            {
                event->HandleEvent();
                event->source->Dispatch(event);
                delete event;
            }
        }
    }
#if USING_MULTICORE
    void Event::BeginThread()
    {
        multicore_launch_core1(&begin_core_1);
    }
#endif

    Event::Event(EventSourceBase* source)
        : source(source)
    {
        if (!is_queue_initialized)
        {
            is_queue_initialized = true;
            queue_init(&event_queue, sizeof(Event*), queue_max_size);
        }
    };
}