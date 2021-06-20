#include "StreamBuffer.h"
#include "HiMPP/ASubsystem/ReadLoop/DataBuffer.h"

#include <cstring>
#include <iostream>

#define PACK_BUFFER_INDEX 0
#define STREAM_BUFFER_INDEX 1

namespace hisilicon::mpp::venc {

VENC_PACK_S *StreamBuffer::getPackBuffer(HI_U32 packsCount) {
    const size_t size = sizeof(VENC_PACK_S) * packsCount;
    int8_t *buf = buffer()->getBuffer(size, PACK_BUFFER_INDEX);
    return reinterpret_cast<VENC_PACK_S *>(buf);
}

HI_U8 *StreamBuffer::getConsecutiveStreamBuffer(const VENC_PACK_S &pack) {
    const size_t totalLength = pack.u32Len[0] + pack.u32Len[1];
    int8_t *buf = buffer()->getBuffer(totalLength, STREAM_BUFFER_INDEX);
    std::memcpy(buf, pack.pu8Addr[0], pack.u32Len[0]);
    std::memcpy(buf + pack.u32Len[0], pack.pu8Addr[1], pack.u32Len[1]);
    //    std::cout << "getConsecutiveStreamBuffer " << totalLength << std::endl;
    return reinterpret_cast<HI_U8 *>(buf);
}

}
