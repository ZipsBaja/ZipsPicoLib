#pragma once

#include <util/TypeUtils.h>
#include <util/FunctionUtils.h>

#include <stdio.h>
#include <stdint.h>
#include <cstddef>
#include <map>
#include <vector>
#include <algorithm>

#include <hardware/irq.h>
#include <pico/util/queue.h>
#include <pico/stdlib.h>

namespace uazips
{
    class Event;

    constexpr size_t queue_max_size = 64;

    class EventSourceBase
    {
    public:
        virtual ~EventSourceBase() = default;
        virtual void Dispatch(const void* ev) = 0;
    };

    enum EventDispatchSystem
    {
        ALL,
        CYCLE
    };

    template<class EventType> requires Extends<EventType, Event>
    class EventSource : public EventSourceBase
    {
    public:
        using EventListener = std::function<void(const EventType*)>;

    protected:
        std::map<const char*, EventListener, functionutils::predicates::CStringCompare> listeners_map;
        std::vector<const char*> listeners;
        queue_t action_queue; // Accepts c strings
        EventDispatchSystem event_system;
        size_t cycle_index;

    public:
        inline EventSource() : event_system(EventDispatchSystem::ALL), cycle_index(0)
        {
            queue_init(&action_queue, sizeof(const char*), queue_max_size);
        }
        virtual inline ~EventSource()
        {
            queue_free(&action_queue);
        }

        inline size_t GetListenerCount() const
        {
            return listeners.size();
        }

        // Override to force action upon adding a callback.
        virtual void OnAddListener() {}
        // Override to force action upon removing a callback.
        virtual void OnRemoveListener() {}

        // Adds an action to be called when the event is fired.
        inline const char* AddListener(const char* listener_name, const EventListener& listener)
        {
            OnAddListener();
            listeners_map.emplace(listener_name, listener);
            listeners.push_back(listener_name);
            return listener_name;
        }
        // Removes an action from being called when the event is fired.
        inline const char* RemoveListener(const char* listener_name)
        {
            OnRemoveListener();
            listeners_map.erase(listener_name);
            listeners.erase(std::remove_if(listeners.begin(), listeners.end(), [&listener_name](const char* s){
                return strcmp(s, listener_name) == 0;
            }));
            return listener_name;
        }

        // Anonymous version; its time of creation is used as its identifier.
        // I do not want to wrap the return in a smart pointer... so delete it.
        inline const char* AddListener(const EventListener& listener)
        {
            char buff[9];
            memset(buff, '\0', 9);
            snprintf(buff, 8, "%llu", to_us_since_boot(get_absolute_time()));
            char* out = new char[strlen(buff) + 1];
            strcpy(out, buff);
            AddListener(out, listener);
            return out;
        }

        void Dispatch(const void* event) override
        {
            EventType* eventptr = (EventType*)event;
            switch (event_system)
            {
                case ALL:
                {
                    for (auto& [key, value] : listeners_map)
                    {
                        value(eventptr);
                    }
                    break;
                }
                case CYCLE:
                {
                    listeners_map[listeners[cycle_index++]](eventptr);
                    if (cycle_index >= listeners.size())
                        cycle_index = 0;
                    break;
                }
            }
        }

        inline void SetDispatchSystem(EventDispatchSystem event_system)
        {
            this->event_system = event_system;
        }

        inline void SetCycleIndex(size_t index)
        {
            cycle_index = index;
        }

        // Creates a listener that takes two other EventSource objects to call one action
        // or a different action depending on which event from the source was fired first.
        // Useful for creating confirmation messages.
        template<class EventType1, class EventType2>
        const char* AddDisjunctionListener(const char* disjunction_name, const char* advance_listener, const char* fallback_listener,
            EventSource<EventType1>& source1, EventSource<EventType2>& source2, const EventListener& listener)
        {
            static constexpr char disjunc1[] = "__disjunc1__";
            static constexpr char disjunc2[] = "__disjunc2__";

            AddListener(disjunction_name, [&](const EventType* event){
                listener(event);
                source1.AddListener(disjunc1, [&](const EventType1* ev){
                    if (listeners_map.contains(advance_listener))
                    {
                        listeners_map[advance_listener](event);
                        auto it = std::find(listeners.begin(), listeners.end(), advance_listener);
                        // Set index to the next action after what was taken from queue.
                        if (it != listeners.end())
                            cycle_index = it - listeners.begin() + 1;
                    }
                    source1.RemoveListener(disjunc1);
                    source2.RemoveListener(disjunc2);
                });
                source2.AddListener(disjunc2, [&](const EventType2* ev){
                    if (listeners_map.contains(fallback_listener))
                    {
                        listeners_map[fallback_listener](event);
                        auto it = std::find(listeners.begin(), listeners.end(), fallback_listener);
                        // Set index to the next action after what was taken from queue.
                        if (it != listeners.end())
                            cycle_index = it - listeners.begin() + 1;
                    }
                    source1.RemoveListener(disjunc1);
                    source2.RemoveListener(disjunc2);
                });
            });
            return disjunction_name;
        }

        // Creates a listener that takes one EventSource object to call one action
        // or a different action depending on if the current source is fired first, or the other.
        // Useful for creatin confirmation messages.
        template<class EventType1>
        const char* AddDisjunctionListener(const char* disjunction_name, const char* advance_listener, const char* fallback_listener,
            EventSource<EventType1>& other_source, const EventListener& listener)
        {
            static constexpr char disjunc1[] = "__disjunc1__";
            static constexpr char disjunc2[] = "__disjunc2__";

            AddListener(disjunction_name, [&](const EventType* event){
                listener(event);
                AddListener(disjunc1, [&](const EventType* ev){
                    if (listeners_map.contains(advance_listener))
                    {
                        listeners_map[advance_listener](event);
                        auto it = std::find(listeners.begin(), listeners.end(), advance_listener);
                        // Set index to the next action after what was taken from queue.
                        if (it != listeners.end())
                            cycle_index = it - listeners.begin() + 1;
                    }
                    RemoveListener(disjunc1);
                    other_source.RemoveListener(disjunc2);
                });
                other_source.AddListener(disjunc2, [&](const EventType1* ev){
                    if (listeners_map.contains(fallback_listener))
                    {
                        listeners_map[fallback_listener](event);
                        auto it = std::find(listeners.begin(), listeners.end(), fallback_listener);
                        // Set index to the next action after what was taken from queue.
                        if (it != listeners.end())
                            cycle_index = it - listeners.begin() + 1;
                    }
                    RemoveListener(disjunc1);
                    other_source.RemoveListener(disjunc2);
                });
            });
            return disjunction_name;
        }

    };

    template<class EventType>
    class IRQHandler : public EventSource<EventType>
    {
    protected:
        absolute_time_t last_time_us;
        uint64_t us_debouncing;

    public:
        inline IRQHandler(uint64_t us_debouncing = 0)
            : EventSource<EventType>(), us_debouncing(us_debouncing), last_time_us(0) {}
        virtual inline ~IRQHandler() = default;
        
        inline void SetDebouncingTime(uint64_t us_debouncing)
        {
            this->us_debouncing = us_debouncing;
        }
        virtual void HandleIRQ(uint32_t events) = 0;
        virtual void Enable() = 0;
        virtual void Disable() = 0;
    };

    class Event
    {
    private:
        static bool is_queue_initialized;

    protected:
        EventSourceBase* source;

    public:
        static queue_t event_queue;
    
        // If mutlicore is enabled, ideally set-up your devices in the second core and call this line to poll them.
        // @param endless_loop Begin a while (true) loop
        static void HandleAllEvents(bool endless_loop);

#if USING_MULTICORE 
        // Will begin the thread to handle events repeatedly.
        static void BeginThread();
#endif

    public:
        Event(EventSourceBase* source);
        virtual ~Event() = default;

        template<class EventSourceType> requires Extends<EventSourceType, EventSourceBase>
        inline EventSourceType* GetSource() const
        {
            return dynamic_cast<EventSourceType*>(source);
        }

        virtual void HandleEvent() {}
    };

    class GPIOEvent : public Event
    {
    protected:
        uint8_t gpio_pin;
        uint32_t events_mask;

    public:
        inline GPIOEvent(EventSourceBase* source, uint8_t gpio_pin, uint32_t events_mask)
            : Event(source), gpio_pin(gpio_pin), events_mask(events_mask) {};

        inline uint8_t GetPin() const
        {
            return gpio_pin;
        }

        inline uint32_t GetEventMask() const
        {
            return events_mask;
        }
    };

    class ButtonEvent : public GPIOEvent
    {
    public:
        inline ButtonEvent(EventSourceBase* source, uint8_t gpio_pin, uint32_t events_mask)
            : GPIOEvent(source, gpio_pin, events_mask) {}
    };

    class TimerEvent : public Event
    {
    public:
        inline TimerEvent(EventSourceBase* source) : Event(source) {}
    };

}