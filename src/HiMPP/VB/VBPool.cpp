#include "VBPool.h"

#include <iostream>

#include <mpi_vb.h>

namespace hisilicon::mpp {

VBPool::VBPool(MPP *p)
    : VBBase(p),
      m_blockSize(0), m_blockCount(0),
      m_id(VB_INVALID_POOLID) {
}

VBPool::~VBPool() {
    if (m_id != VB_INVALID_POOLID) {
        HI_MPI_VB_DestroyPool(m_id);
        std::cout << "~VBPool " << this << " " << m_blockSize << " , " << m_blockCount
                  << std::endl;
    }
}

bool VBPool::configureImpl() {
    if (m_blockCount == 0)
        throw std::runtime_error("m_blockCount must be > 0");

    if (m_blockSize == 0)
        throw std::runtime_error("m_blockSize must be > 0");

    m_id = HI_MPI_VB_CreatePool(m_blockSize, m_blockCount,
                                m_name.empty() ? nullptr : m_name.c_str());

    if (m_id == VB_INVALID_POOLID)
        throw std::runtime_error("HI_MPI_VB_CreatePool failed");

    std::cout << "VBPool created " << this << " " << m_blockSize << " , " <<
              m_blockCount << std::endl;
    return true;
}

void VBPool::setName(const std::string &name) {
    m_name = name;
}

void VBPool::setBlockCount(const HI_U32 &blockCount) {
    m_blockCount = blockCount;
}

void VBPool::setBlockSize(const HI_U32 &blockSize) {
    m_blockSize = blockSize;
}

VB_POOL VBPool::id() const {
    return m_id;
}

}
