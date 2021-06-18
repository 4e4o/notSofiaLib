#include "ReaderMemOut.h"
#include "ReaderFileOut.h"

#include <cstring>
#include <iostream>

namespace hisilicon::mpp {

using namespace std;

ReaderMemOut::ReaderMemOut(size_t s)
    : m_buffer(new HI_U8[s], s), m_pos(0) {
}

ReaderMemOut::~ReaderMemOut() {
    delete[] m_buffer.data();
}

void ReaderMemOut::write(const HI_U8 *data, const HI_U32 &size) {
    if ((m_pos + size) >= m_buffer.size()) {
        std::cout << "ReaderMemOut::write buffer is full " << this << std::endl;
        return;
    }

    std::memcpy(m_buffer.data() + m_pos, data, size);
    m_pos += size;
}

std::span<HI_U8> ReaderMemOut::buffer() const {
    return std::span<HI_U8>(m_buffer.data(), m_pos);
}

}
