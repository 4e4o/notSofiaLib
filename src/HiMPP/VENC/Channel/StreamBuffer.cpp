#include "StreamBuffer.h"

#include <cstring>
#include <iostream>

namespace hisilicon::mpp::venc {

StreamBuffer::StreamBuffer() {
}

StreamBuffer::~StreamBuffer() {
    freePackBuffer();
    freeStreamBuffer();
}

VENC_PACK_S *StreamBuffer::getPackBuffer(HI_U32 packsCount) {
    if (m_packBuffer.size() < packsCount) {
        freePackBuffer();
        m_packBuffer = std::move(std::span(new VENC_PACK_S[packsCount], packsCount));
        //    std::cout << "getPackBuffer new " << packsCount << std::endl;
    }

    return m_packBuffer.data();
}

HI_U8 *StreamBuffer::getConsecutiveStreamBuffer(int packIndex) {
    VENC_PACK_S &pack = m_packBuffer[packIndex];
    const HI_U32 totalLength = pack.u32Len[0] + pack.u32Len[1];

    if (m_streamBuffer.size() < totalLength) {
        freeStreamBuffer();
        m_streamBuffer = std::move(std::span(new HI_U8[totalLength], totalLength));
        //    std::cout << "getConsecutveStreamBuffer new " << totalLength << std::endl;
    }

    //    std::cout << "getConsecutveStreamBuffer memcpy " << totalLength << std::endl;

    std::memcpy(m_streamBuffer.data(), pack.pu8Addr[0], pack.u32Len[0]);
    std::memcpy(m_streamBuffer.data() + pack.u32Len[0], pack.pu8Addr[1],
                pack.u32Len[1]);

    return m_streamBuffer.data();
}

void StreamBuffer::freeStreamBuffer() {
    if (m_streamBuffer.empty())
        return;

    delete[] m_streamBuffer.data();
    m_streamBuffer = std::move(std::span<HI_U8>());
}

void StreamBuffer::freePackBuffer() {
    if (m_packBuffer.empty())
        return;

    delete[] m_packBuffer.data();
    m_packBuffer = std::move(std::span<VENC_PACK_S>());
}

}
