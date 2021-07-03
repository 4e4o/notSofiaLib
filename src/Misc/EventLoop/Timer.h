#ifndef EVENT_LOOP_TIMER_H
#define EVENT_LOOP_TIMER_H

#include "Epollable.h"
#include "Misc/Holder.h"

#include <functional>
#include <chrono>

class Timer : public Holder<EpollLoop *>, public Epollable {
  public:
    typedef std::function<void(Timer *)> TTickEvent;

    Timer(EpollLoop *);
    ~Timer();

    template<class Callable>
    void setTick(Callable &&c) {
        m_tick = std::move(c);
    }

    bool start(const std::chrono::milliseconds &t);
    bool stop();

    bool oneShot() const;
    void setOneShot(bool oneShot);

  private:
    void onRead() override final;
    void open();
    void close();

    int m_fd;
    TTickEvent m_tick;
    bool m_oneShot;
};

#endif // EVENT_LOOP_TIMER_H
