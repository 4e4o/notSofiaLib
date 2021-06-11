#include "VBPool.h"
#include "HiMPP/VENC/Subsystem.h"
#include "HiMPP/VENC/Group.h"
#include "HiMPP/VENC/Channel/Channel.h"

#include <iostream>

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

void VBPool::initForVenc() {
    int channelsCount = 0;
    const HI_U32 maxSize = maxVencBlkSize(channelsCount);
    const int veduCount = parent()->veduCount();

    setBlockSize(maxSize);

    // HiMPP Media Processing Software Development Reference.pdf
    // page 512
    // TODO
    // BlkCnt = n + 2 x VEDU. If the 4x frame skipping reference mode, advanced frame skipping
    // reference mode, or SVC-T encoding is configured for a channel, an extra frame buffer is
    // required for this channel.
    setBlockCount(channelsCount + 2 * veduCount + EXTRA_BLOCKS_COUNT);
}

HI_U32 VBPool::maxVencBlkSize(int &channelsCount) {
    channelsCount = 0;
    const venc::Subsystem *venc = parent()->venc();

    if (venc == nullptr)
        return 0;

    HI_U32 max = 0, tmp = 0;

    for (auto &group : venc->groups()) {
        for (auto &channel : group->channels()) {
            if (!channel->needUserPool())
                continue;

            tmp = picVbBlkSize(channel);
            channelsCount++;

            if (tmp > max)
                max = tmp;
        }
    }

    return max;
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
