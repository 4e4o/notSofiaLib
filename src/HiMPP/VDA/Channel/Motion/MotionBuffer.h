#ifndef MOTION_BUFFER_H
#define MOTION_BUFFER_H

#include "HiMPP/ASubsystem/ReadLoop/DataBufferWrapper.h"

#include <hi_comm_vda.h>

namespace hisilicon::mpp::vda {

// Враппер над DataBuffer

class MotionBuffer : public DataBufferWrapper {
  public:
    using DataBufferWrapper::DataBufferWrapper;

};

}

#endif // MOTION_BUFFER_H

