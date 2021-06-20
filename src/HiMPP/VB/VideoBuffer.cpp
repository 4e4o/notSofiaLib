#include "VideoBuffer.h"
#include "HiMPP/VI/Subsystem.h"
#include "HiMPP/VI/Device.h"
#include "HiMPP/VI/Channel.h"

#include <iostream>

#include <mpi_vb.h>

#define DEFAULT_MAX_POOL_COUNT 128

#define MAIN_POOL_INDEX 0

namespace hisilicon::mpp {

static void addPool(VB_CONF_S &conf, int i, HI_U32 blockSize, HI_U32 blkCount) {
    conf.astCommPool[i].u32BlkSize = blockSize;
    conf.astCommPool[i].u32BlkCnt = blkCount;
}

VideoBuffer::~VideoBuffer() {
    HI_MPI_VB_Exit();
    std::cout << "~VideoBuffer " << this << std::endl;
}

bool VideoBuffer::configureImpl() {
    HI_U32 channelsCount = 0;
    const HI_U32 blockSize = maxViBlkSize(channelsCount);

    if (channelsCount < 1)
        throw std::runtime_error("No channels to process");

    if (blockSize < 1)
        throw std::runtime_error("Invalid block size");

    VB_CONF_S stVbConf{};

    stVbConf.u32MaxPoolCnt = DEFAULT_MAX_POOL_COUNT;

    // основной пул для vi, vpss и других сервисов у которых нет апи приатачивания к пулу
    addPool(stVbConf, MAIN_POOL_INDEX, blockSize,
            getBlocksCount(channelsCount) + EXTRA_BLOCKS_COUNT);

    if (HI_MPI_VB_SetConf(&stVbConf) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VB_SetConf failed");

    if (HI_MPI_VB_Init() != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VB_Init failed");

    return true;
}

HI_U32 VideoBuffer::getBlocksCount(const HI_U32 &channelsCount) {
    // Определяется экспериментальным путём, возможно это частный случай
    // поэтому метод сделан виртуальным.
    return channelsCount * 4 + 1;
}

HI_U32 VideoBuffer::maxViBlkSize(HI_U32 &channelsCount) {
    channelsCount = 0;
    const vi::Subsystem *vi = parent()->vi();

    if (vi == nullptr)
        return 0;

    HI_U32 max = 0, tmp = 0;

    for (auto &device : vi->devices()) {
        for (auto &channel : device->channels()) {
            channelsCount++;
            std::unique_ptr<const IFrameFormatSource> info(channel->vbFormatInfo());
            tmp = picVbBlkSize(info.get());

            if (tmp > max)
                max = tmp;
        }
    }

    return max;
}

}
