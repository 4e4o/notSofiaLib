#ifndef READ_LOOP_DATA_BUFFER_WRAPPER_H
#define READ_LOOP_DATA_BUFFER_WRAPPER_H

#include "Misc/Holder.h"

namespace hisilicon::mpp {

class DataBuffer;

class DataBufferWrapper : public Holder<DataBuffer *> {
  public:
    using Holder::Holder;

    DataBuffer *buffer() const {
        return Holder::value();
    }
};


}

#endif // READ_LOOP_DATA_BUFFER_WRAPPER_H

