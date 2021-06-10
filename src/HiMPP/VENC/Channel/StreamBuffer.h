#ifndef STREAM_BUFFER__H
#define STREAM_BUFFER__H

#include <hi_comm_venc.h>

namespace hisilicon::mpp::venc {

class StreamBuffer {
public:
    StreamBuffer();
    ~StreamBuffer();

    VENC_PACK_S* get(HI_U32 packsCount);
    HI_U32 size() const;

private:
    void freeBuffer();

    VENC_PACK_S* m_buffer;
    HI_U32 m_currentPacks;
};

}

#endif // STREAM_BUFFER__H

