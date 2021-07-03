#include "LoopReader.h"
#include "ReadLoop.h"
#include "IReaderOut.h"
#include "DataBufferWrapper.h"

#include <iostream>

namespace hisilicon::mpp {

LoopReader::LoopReader(IdHolder *h)
    : Holder(h), m_ownsOut(true),
      m_fd(INVALID_FD) {
}

LoopReader::~LoopReader() {
    releaseOut();
}

void LoopReader::setOut(IReaderOut *out, bool getOwnership) {
    releaseOut();
    m_out.reset(out);
    m_ownsOut = getOwnership;
}

void LoopReader::attach(int fd, ReadLoop *loop) {
    if (fd < 0)
        throw std::runtime_error("[LoopReader] invalid fd");

    m_fd = fd;
    m_buffer.reset(createBufferWrapper(loop->buffer()));
    attachToLoop(loop, EpoolOperationType{.READ = 1}, m_fd);
}

int LoopReader::id() const {
    return Holder::value()->id();
}

void LoopReader::write(const HI_U8 *data, const HI_U32 &size) {
    m_out->write(data, size);
}

void LoopReader::releaseOut() {
    if (!m_ownsOut)
        m_out.release();
}

}
