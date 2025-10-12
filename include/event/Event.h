#pragma once

#include <util/TypeUtils.h>

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <cstddef>
#include <functional>
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
        CYCLE,
        QUEUE
    };

    template<class EventType> requires Extends<EventType, Event>
    class EventSource : public EventSourceBase
    {
    public:
        using EventListener = std::function<void(const EventType*)>;

    protected:
        std::map<const char*, EventListener> listeners_map;
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

        // Adds an action to be called when the event is fired.
        inline const char* AddListener(const char* listener_name, const EventListener& listener)
        {
            listeners_map[listener_name] = listener;
            listeners.push_back(listener_name);
            return listener_name;
        }
        // Removes an action from being called when the event is fired.
        inline const char* RemoveListener(const char* listener_name)
        {
            listeners_map.erase(listener_name);
            std::erase(listeners, listener_name);
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
                        value(eventptr);
                    break;
                }
                case CYCLE:
                {
                    listeners_map[listeners[cycle_index++]](eventptr);
                    if (cycle_index >= listeners.size())
                        cycle_index = 0;
                    break;
                }
                case QUEUE:
                {
                    const char* action;
                    if (queue_try_remove(&action_queue, &action))
                    {
                        if (listeners_map.contains(action))
                        {
                            listeners_map[action](eventptr);
                            auto it = std::find(listeners.begin(), listeners.end(), action);
                            // Set index to the next action after what was taken from queue.
                            if (it != listeners.end())
                                cycle_index = it - listeners.begin() + 1;
                        }
                    }
                    else
                    {
                        // fallback to cycle system
                        listeners_map[listeners[cycle_index++]](eventptr);
                        if (cycle_index >= listeners.size())
                            cycle_index = 0;
                    }
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
        // Ideally, the QUEUE dispatch system should be set for this to have its full functionality.
        template<class EventType1, class EventType2>
        const char* AddDisjunctionListener(const char* advance_listener, const char* fallback_listener,
            EventSource<EventType1>& source1, EventSource<EventType2>& source2,
            const char* disjunction_name, const EventListener& listener)
        {
            AddListener(disjunction_name, [&](const EventType* event){
                listener(event);
                source1.AddListener("__disjunc1__", [&](const EventType1* ev){
                    queue_try_add(&action_queue, &advance_listener);
                    source1.RemoveListener("__disjunc1__");
                    source2.RemoveListener("__disjunc2__");
                });
                source2.AddListener("__disjunc2__", [&](const EventType2* ev){
                    queue_try_add(&action_queue, &fallback_listener);
                    source1.RemoveListener("__disjunc1__");
                    source2.RemoveListener("__disjunc2__");
                });
            });
            return disjunction_name;
        }

        // Creates a listener that takes one EventSource object to call one action
        // or a different action depending on if the current source is fired first, or the other.
        // Useful for creatin confirmation messages.
        // Ideally, the QUEUE dispatch system should be set for this to have its full functionality.
        template<class EventType1>
        void AddDisjunctionListener(const char* advance_listener, const char* fallback_listener,
            EventSource<EventType1>& other_source, const char* disjunction_name, const EventListener& listener)
        {
            AddListener(disjunction_name, [&](const EventType* event){
                listener(event);
                AddListener("__disjunc1__", [&](const EventType* ev){
                    queue_try_add(&action_queue, &advance_listener);
                    RemoveListener("__disjunc1__");
                    other_source.RemoveListener("__disjunc2__");
                });
                other_source.AddListener("__disjunc2__", [&](const EventType1* ev){
                    queue_try_add(&action_queue, &fallback_listener);
                    RemoveListener("__disjunc1__");
                    other_source.RemoveListener("__disjunc2__");
                });
            });
        }

    };

    template<class EventType>
    class IRQHandler : public EventSource<EventType>
    {
    public:
        inline IRQHandler() : EventSource<EventType>() {}
        virtual inline ~IRQHandler() = default;
        
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
        // @param us_debouncing Amount of microseconds to delay the event checking. This is a software solution to debouncing.
        static void HandleAllEvents(bool endless_loop, uint32_t us_debouncing = 0);

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

    public:
        inline GPIOEvent(EventSourceBase* source, uint8_t gpio_pin) : Event(source) {};

        inline uint8_t GetPin() const
        {
            return gpio_pin;
        }
    };

    class ButtonEvent : public GPIOEvent
    {
    public:
        inline ButtonEvent(EventSourceBase* source, uint8_t gpio_pin) : GPIOEvent(source, gpio_pin) {}
    };

    class TimerEvent : public Event
    {
    public:
        inline TimerEvent(EventSourceBase* source) : Event(source) {}
    };

}