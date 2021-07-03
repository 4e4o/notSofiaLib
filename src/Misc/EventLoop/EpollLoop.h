#ifndef EPOLL_LOOP_H
#define EPOLL_LOOP_H

#include "EpoolOperationType.h"

#include <atomic>

class Epollable;

class EpollLoop {
  public:
    EpollLoop();
    ~EpollLoop();

    void run();
    virtual void stop();

    bool add(EpoolOperationType, Epollable *, int fd);
    bool remove(Epollable *, int fd);

  protected:
    virtual void iteration();

  private:
    bool createEpoll();

    int m_fd;
    std::atomic<bool> m_running;
};

#endif // EPOLL_LOOP_H
