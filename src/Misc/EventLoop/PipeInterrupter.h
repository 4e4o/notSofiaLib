#ifndef PIPE_INTERRUPTER_H
#define PIPE_INTERRUPTER_H

#include "Epollable.h"

class EpollLoop;

class PipeInterrupter : public Epollable {
  public:
    PipeInterrupter(EpollLoop *);
    ~PipeInterrupter();

    void interrupt();

  private:
    virtual void onRead() override final;
    void open();
    void close();

    int m_fds[2];
};

#endif // PIPE_INTERRUPTER_H
