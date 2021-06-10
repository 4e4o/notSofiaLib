#include "StreamBuffer.h"

namespace hisilicon::mpp::venc {

StreamBuffer::StreamBuffer()
    : m_buffer(nullptr),
      m_currentPacks(0) {
}

StreamBuffer::~StreamBuffer() {
    freeBuffer();
}

HI_U32 StreamBuffer::size() const {
    return sizeof(VENC_PACK_S) * m_currentPacks;
}

VENC_PACK_S* StreamBuffer::get(HI_U32 packsCount) {
    if (m_currentPacks < packsCount) {
        freeBuffer();
        m_buffer = new VENC_PACK_S[packsCount];
        m_currentPacks = packsCount;
    }

    return m_buffer;
}

void StreamBuffer::freeBuffer() {
    if (m_buffer == nullptr)
        return;

    delete[] m_buffer;
    m_currentPacks = 0;
}

}
