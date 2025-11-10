#pragma once

#include "IODevice.h"

namespace uazips
{

    class DualActionSensor : public IODevice<GPIOEvent>
    {
    public:
        DualActionSensor(uint8_t gpio_pin);

        const char* AddEdgeRiseListener(const char* name, const EventListener& action);
        const char* AddEdgeFallListener(const char* name, const EventListener& action);

        template<class EventType1, class EventType2>
        const char* AddEdgeRiseDisjunctionListener(const char* disjunction_name, const char* advance_listener, const char* fallback_listener,
            EventSource<EventType1>& source1, EventSource<EventType2>& source2, const EventListener& listener)
        {
            return AddEdgeRiseListener(disjunction_name, [&](const GPIOEvent* event){
                source1.AddListener(name_disjunc1, [&](const EventType1* ev){
                    if (listeners_map.contains(advance_listener))
                    {
                        listeners_map[advance_listener].listener(event);
                        auto it = std::find(listeners.begin(), listeners.end(), advance_listener);
                        // Set index to the next action after what was taken from queue.
                        if (it != listeners.end())
                            cycle_index = it - listeners.begin() + 1;
                    }
                    source1.RemoveListener(name_disjunc1);
                    source2.RemoveListener(name_disjunc2);
                });
                source2.AddListener(name_disjunc2, [&](const EventType2* ev){
                    if (listeners_map.contains(fallback_listener))
                    {
                        listeners_map[fallback_listener].listener(event);
                        auto it = std::find(listeners.begin(), listeners.end(), fallback_listener);
                        // Set index to the next action after what was taken from queue.
                        if (it != listeners.end())
                            cycle_index = it - listeners.begin() + 1;
                    }
                    source1.RemoveListener(name_disjunc1);
                    source2.RemoveListener(name_disjunc2);
                });
                listener(event);
            });
        }

        template<class EventType1, class EventType2>
        const char* AddEdgeFallDisjunctionListener(const char* disjunction_name, const char* advance_listener, const char* fallback_listener,
            EventSource<EventType1>& source1, EventSource<EventType2>& source2, const EventListener& listener)
        {
            return AddEdgeFallListener(disjunction_name, [&](const GPIOEvent* event){
                source1.AddListener(name_disjunc1, [&](const EventType1* ev){
                    if (listeners_map.contains(advance_listener))
                    {
                        listeners_map[advance_listener].listener(event);
                        auto it = std::find(listeners.begin(), listeners.end(), advance_listener);
                        // Set index to the next action after what was taken from queue.
                        if (it != listeners.end())
                            cycle_index = it - listeners.begin() + 1;
                    }
                    source1.RemoveListener(name_disjunc1);
                    source2.RemoveListener(name_disjunc2);
                });
                source2.AddListener(name_disjunc2, [&](const EventType2* ev){
                    if (listeners_map.contains(fallback_listener))
                    {
                        listeners_map[fallback_listener].listener(event);
                        auto it = std::find(listeners.begin(), listeners.end(), fallback_listener);
                        // Set index to the next action after what was taken from queue.
                        if (it != listeners.end())
                            cycle_index = it - listeners.begin() + 1;
                    }
                    source1.RemoveListener(name_disjunc1);
                    source2.RemoveListener(name_disjunc2);
                });
                listener(event);
            });
        }

        template<class EventType>
        const char* AddEdgeRiseDisjunctionListener(const char* disjunction_name, const char* advance_listener, const char* fallback_listener,
            EventSource<EventType>& other_source, const EventListener& listener)
        {
            return AddEdgeRiseListener(disjunction_name, [&](const GPIOEvent* event){
                AddListener(name_disjunc1, [&](const GPIOEvent* ev){
                    if (listeners_map.contains(advance_listener))
                    {
                        listeners_map[advance_listener].listener(event);
                        auto it = std::find(listeners.begin(), listeners.end(), advance_listener);
                        // Set index to the next action after what was taken from queue.
                        if (it != listeners.end())
                            cycle_index = it - listeners.begin() + 1;
                    }
                    RemoveListener(name_disjunc1);
                    other_source.RemoveListener(name_disjunc2);
                });
                other_source.AddListener(name_disjunc2, [&](const EventType* ev){
                    if (listeners_map.contains(fallback_listener))
                    {
                        listeners_map[fallback_listener].listener(event);
                        auto it = std::find(listeners.begin(), listeners.end(), fallback_listener);
                        // Set index to the next action after what was taken from queue.
                        if (it != listeners.end())
                            cycle_index = it - listeners.begin() + 1;
                    }
                    RemoveListener(name_disjunc1);
                    other_source.RemoveListener(name_disjunc2);
                });
                listener(event);
            });
        }

        template<class EventType>
        const char* AddEdgeFallDisjunctionListener(const char* disjunction_name, const char* advance_listener, const char* fallback_listener,
            EventSource<EventType>& other_source, const EventListener& listener)
        {
            return AddEdgeFallListener(disjunction_name, [&](const GPIOEvent* event){
                AddListener(name_disjunc1, [&](const GPIOEvent* ev){
                    if (listeners_map.contains(advance_listener))
                    {
                        listeners_map[advance_listener].listener(event);
                        auto it = std::find(listeners.begin(), listeners.end(), advance_listener);
                        // Set index to the next action after what was taken from queue.
                        if (it != listeners.end())
                            cycle_index = it - listeners.begin() + 1;
                    }
                    RemoveListener(name_disjunc1);
                    other_source.RemoveListener(name_disjunc2);
                });
                other_source.AddListener(name_disjunc2, [&](const EventType* ev){
                    if (listeners_map.contains(fallback_listener))
                    {
                        listeners_map[fallback_listener].listener(event);
                        auto it = std::find(listeners.begin(), listeners.end(), fallback_listener);
                        // Set index to the next action after what was taken from queue.
                        if (it != listeners.end())
                            cycle_index = it - listeners.begin() + 1;
                    }
                    RemoveListener(name_disjunc1);
                    other_source.RemoveListener(name_disjunc2);
                });
                listener(event);
            });
        }

        virtual void HandleIRQ(uint32_t events) override;
    };

}