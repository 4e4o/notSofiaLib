#ifndef MPP_READ_SELECT_LOOP_H
#define MPP_READ_SELECT_LOOP_H

#include "Misc/SelectReadLoop.h"

#include <memory>

namespace hisilicon::mpp {

class DataBuffer;

class ReadLoop : public SelectReadLoop {
  public:
    ReadLoop();
    ~ReadLoop();

    DataBuffer *buffer() const;

  private:
    void onTimeout() override final;

    std::unique_ptr<DataBuffer> m_buffer;
};

}

#endif // MPP_READ_SELECT_LOOP_H

