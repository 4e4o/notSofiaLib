#ifndef EPOLLABLE_H
#define EPOLLABLE_H

#define INVALID_FD -1

#include "EpoolOperationType.h"

class EpollLoop;

class Epollable {
  public:
    Epollable();
    virtual ~Epollable();

    virtual void onRead();
    virtual void onWrite();
    virtual void onError();

    EpollLoop *loop() const;

    void attachToLoop(EpollLoop *, EpoolOperationType, int fd);
    void removeFromLoop(int fd);

  private:
    EpollLoop *m_loop;
};

#endif // EPOLLABLE_H
