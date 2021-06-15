#include "VencVBPool.h"
#include "HiMPP/VENC/Subsystem.h"
#include "HiMPP/VENC/Group.h"
#include "HiMPP/VENC/Channel/Channel.h"

namespace hisilicon::mpp::venc {

VBPool::VBPool(MPP *p)
    : mpp::VBPool(p) {
    init();
}

void VBPool::init() {
    int channelsCount = 0;
    const HI_U32 maxSize = maxBlkSize(channelsCount);
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

HI_U32 VBPool::maxBlkSize(int &channelsCount) {
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

}
