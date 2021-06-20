#ifndef STREAM_BUFFER__H
#define STREAM_BUFFER__H

#include "HiMPP/ASubsystem/ReadLoop/DataBufferWrapper.h"

#include <hi_comm_venc.h>

namespace hisilicon::mpp::venc {

// Враппер над DataBuffer

class StreamBuffer : public DataBufferWrapper {
  public:
    using DataBufferWrapper::DataBufferWrapper;

    VENC_PACK_S *getPackBuffer(HI_U32 packsCount);
    HI_U8 *getConsecutiveStreamBuffer(const VENC_PACK_S &pack);
};

}

#endif // STREAM_BUFFER__H

