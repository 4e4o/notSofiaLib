#include "DataBuffer.h"

#include <iostream>

#define INITIAL_BUFFERS_COUNT 1

namespace hisilicon::mpp {

DataBuffer::DataBuffer() :
    m_buffers(INITIAL_BUFFERS_COUNT) {
}

DataBuffer::~DataBuffer() {
    freeBuffers();
}

void DataBuffer::addBuffersIfNecessary(int index) {
    const int needSize = index + 1;
    const int curSize = m_buffers.size();

    if (curSize >= needSize)
        return;

    const int toAdd = needSize - curSize;

    for (int i = 0 ; i < toAdd ; i++)
        m_buffers.emplace_back();
}

int8_t *DataBuffer::getBuffer(const size_t &size, int index) {
    addBuffersIfNecessary(index);
    TBuffer &buffer = m_buffers[index];

    if (buffer.size() < size) {
        freeBuffer(index);
        buffer = std::move(std::span(new int8_t[size], size));
        //        std::cout << "DataBuffer::getBuffer new "
        //                  << index << " , " << size << std::endl;
    }

    return buffer.data();
}

void DataBuffer::freeBuffers() {
    for (int i = 0 ; i < (int) m_buffers.size() ; i++)
        freeBuffer(i);
}

void DataBuffer::freeBuffer(int index) {
    TBuffer &buffer = m_buffers[index];

    if (buffer.empty())
        return;

    delete[] buffer.data();
    buffer = std::move(std::span<int8_t>());
}

}
