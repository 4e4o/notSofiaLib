#ifndef STREAM_BUFFER__H
#define STREAM_BUFFER__H

#include <span>

#include <hi_comm_venc.h>

namespace hisilicon::mpp::venc {

class StreamBuffer {
  public:
    StreamBuffer();
    ~StreamBuffer();

    VENC_PACK_S *getPackBuffer(HI_U32 packsCount);
    HI_U8 *getConsecutiveStreamBuffer(int packIndex);

  private:
    void freePackBuffer();
    void freeStreamBuffer();

    std::span<VENC_PACK_S> m_packBuffer;
    std::span<HI_U8> m_streamBuffer;
};

}

#endif // STREAM_BUFFER__H

