#include "ReadLoop.h"
#include "DataBuffer.h"

#include <iostream>

namespace hisilicon::mpp {

ReadLoop::ReadLoop() :
    m_buffer(new DataBuffer()) {
}

ReadLoop::~ReadLoop() {
}

DataBuffer *ReadLoop::buffer() const {
    return m_buffer.get();
}

void ReadLoop::onTimeout() {
    SelectReadLoop::onTimeout();
    std::cout << "[hisilicon::mpp::ReadLoop] select failed!" << std::endl;
}

}
