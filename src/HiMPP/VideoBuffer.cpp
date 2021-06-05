#include "VideoBuffer.h"
#include "VI/Subsystem.h"
#include "VI/Device.h"
#include "VI/Channel.h"
#include "Sys/Sys.h"

#include <string.h>
#include <stdexcept>
#include <cmath>
#include <iostream>

#include <mpi_vb.h>

#define DEFAULT_MAX_POOL_COUNT 128

namespace hisilicon {
namespace mpp {

static void setPool(VB_CONF_S& conf, int i, HI_U32 blockSize, HI_U32 blkCount) {
    conf.astCommPool[i].u32BlkSize = blockSize;
    conf.astCommPool[i].u32BlkCnt = blkCount;
    memset(conf.astCommPool[i].acMmzName, 0,
           sizeof(conf.astCommPool[i].acMmzName));
}

VideoBuffer::VideoBuffer(MPP* mpp)
    : MPPChild(mpp) {
}

VideoBuffer::~VideoBuffer() {    
    HI_MPI_VB_Exit();
    std::cout << "~VideoBuffer " << this << std::endl;
}

bool VideoBuffer::startImpl() {
    // на стадии конфигурации нет нужных данных
    // они есть только на стадии запуска
    HI_U32 channelsCount = 0;
    const vi::Subsystem* vi = parent()->vi();

    if (vi != NULL)
        channelsCount = vi->channelsCount();

    if (channelsCount < 1)
        throw std::runtime_error("No channels to process");

    const HI_U32 blockSize = maxPicVbBlkSize();

    if (blockSize < 1)
        throw std::runtime_error("Invalid block size");

    VB_CONF_S stVbConf{};

    stVbConf.u32MaxPoolCnt = DEFAULT_MAX_POOL_COUNT;

    // TODO why * 4 ??
    // < 3 not works

    // Значит тут имеется зависимость от кол-ва каналов на группу в vpps
    // Сначало было 4 канала на группу, 4 группы vpps и работало только с channelCount * 4
    // Потом сделал 1 канал на группу, 4 группы, стало работать с channelCount * 3,
    // - с 2 уже не работает, зависимость пока не понятна

    setPool(stVbConf, 0, blockSize, channelsCount * 4);

    if (HI_MPI_VB_SetConf(&stVbConf) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VB_SetConf failed");

    if (HI_MPI_VB_Init() != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VB_Init failed");

    return true;
}

HI_U32 VideoBuffer::maxPicVbBlkSize() {
    HI_U32 result = 0, tmp;
    const vi::Subsystem* vi = parent()->vi();

    if (vi == NULL)
        return result;

    auto& devs = vi->devices();

    for (int i = 0 ; i < (int) devs.size() ; i++) {
        auto& channels = devs[i]->channels();

        for (int j = 0 ; j < (int) channels.size() ; j++) {
            tmp = picVbBlkSize(channels[j]);

            std::cout << i << ", " << j << " channel video block size = " << tmp << std::endl;

            if (result < tmp)
                result = tmp;
        }
    }

    return result;
}

// размер буфер страйда должен быть кратен sysAlignWidth
// HiMPP Media Processing Software Development Reference.pdf
// page 90

HI_U32 VideoBuffer::picVbBlkSize(vi::Channel *ch) {
    const HI_U32 sysAlignWidth = parent()->sys()->sysWidthAlign();
    const SIZE_S imgSize = ch->imgSize();
    const PIXEL_FORMAT_E pixFmt = ch->pixelFormat();

    if ((PIXEL_FORMAT_YUV_SEMIPLANAR_422 != pixFmt) &&
            (PIXEL_FORMAT_YUV_SEMIPLANAR_420 != pixFmt)) {
        throw std::runtime_error("Unsupported pixel format");
    }

    // https://stackoverflow.com/questions/8561185/yuv-422-yuv-420-yuv-444
    // https://www.fourcc.org/yuv.php
    // https://wiki.videolan.org/YUV/
    // 16 bits for 422, 12 bits for 420
    const float bytesPerPixel = (PIXEL_FORMAT_YUV_SEMIPLANAR_422 == pixFmt) ? 2 : 1.5;
    const HI_U32 bufSize = std::ceil(imgSize.u32Width * imgSize.u32Height * bytesPerPixel);

    // потому что степень для CEILING_2_POWER должна быть кратна 2
    if (sysAlignWidth > 1)
        return CEILING_2_POWER(bufSize, sysAlignWidth);

    return bufSize;
}

}
}
