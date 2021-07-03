#ifndef MPP_READ_LOOP_H
#define MPP_READ_LOOP_H

#include "Misc/EventLoop/EventLoop.h"

#include <memory>

namespace hisilicon::mpp {

class DataBuffer;

class ReadLoop : public EventLoop {
  public:
    ReadLoop();
    ~ReadLoop();

    DataBuffer *buffer() const;

  private:
    std::unique_ptr<DataBuffer> m_buffer;
};

}

#endif // MPP_READ_LOOP_H

