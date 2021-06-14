#ifndef STREAM_BUFFER__H
#define STREAM_BUFFER__H

#include <hi_comm_venc.h>

namespace hisilicon::mpp {
class DataBuffer;
}

namespace hisilicon::mpp::venc {

// Враппер над DataBuffer

class StreamBuffer {
  public:
    StreamBuffer(DataBuffer *);

    VENC_PACK_S *getPackBuffer(HI_U32 packsCount);
    HI_U8 *getConsecutiveStreamBuffer(const VENC_PACK_S &pack);

  private:
    DataBuffer *m_buffer;
};

}

#endif // STREAM_BUFFER__H

