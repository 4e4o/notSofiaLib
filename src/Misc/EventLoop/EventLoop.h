#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include "EpollLoop.h"
#include "EventQueue.h"

#include <memory>

class PipeInterrupter;

class EventLoop : public EpollLoop, public EventQueue {
  public:
    EventLoop();
    ~EventLoop();

    void stop() override;

  private:
    void onPost() override final;
    void iteration() override final;

    std::unique_ptr<PipeInterrupter> m_interrupter;
};

#endif // EVENT_LOOP_H
