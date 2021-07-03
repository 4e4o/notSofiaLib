#include "ReadLoop.h"
#include "DataBuffer.h"

namespace hisilicon::mpp {

ReadLoop::ReadLoop() :
    m_buffer(new DataBuffer()) {
}

ReadLoop::~ReadLoop() {
}

DataBuffer *ReadLoop::buffer() const {
    return m_buffer.get();
}

}
