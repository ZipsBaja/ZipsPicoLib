#include <event/Event.h>
#include <ZipsLib.h>
#include <util/TimeHandler.h>

#if USING_MULTICORE
#include <pico/multicore.h>
#endif

namespace uazips
{

    bool Event::is_queue_initialized = false;
    queue_t Event::event_queue = {};

    void Event::HandleAllEvents(bool endless_loop, uint32_t us_debouncing)
    {
        if (endless_loop)
        {
            if (us_debouncing)
            {
                TimeHandler th;
                uint64_t cumulative = 0;
                while (1)
                {
                    th.Update();
                    cumulative += th.DeltaTime;
                    if (cumulative >= us_debouncing)
                    {
                        cumulative -= us_debouncing;
                        Event* event;
                        queue_remove_blocking(&event_queue, &event);
                        if (event)
                        {
                            event->HandleEvent();
                            delete event;
                        }
                    }
                }
            }
            else
            {
                while (1)
                {
                    Event* event;
                    queue_remove_blocking(&event_queue, &event);
                    if (event)
                    {
                        event->HandleEvent();
                        delete event;
                    }
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
                delete event;
            }
        }
    }

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