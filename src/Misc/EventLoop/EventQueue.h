#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <mutex>
#include <list>
#include <functional>

class EventQueue {
  public:
    typedef std::function<void()> TEvent;

    EventQueue();
    ~EventQueue();

    template<class Callable>
    void post(Callable &&c) {
        {
            std::lock_guard l(m_eventsMutex);
            m_events.emplace_back(std::forward<Callable>(c));
        }

        onPost();
    }

  protected:
    void process();

  private:
    virtual void onPost() { }
    typedef std::list<TEvent> TEvents;

    std::mutex m_eventsMutex;
    TEvents m_events;
};

#endif // EVENT_QUEUE_H
