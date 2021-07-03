#include "EventQueue.h"

EventQueue::EventQueue() {
}

EventQueue::~EventQueue() {
}

void EventQueue::process() {
    TEvents events;

    {
        std::lock_guard l(m_eventsMutex);
        events = std::move(m_events);
        m_events = {};
    }

    for (TEvent &event : events)
        event();
}
