#include "VideoBuffer.h"
#include "Vi/Source/ViInfoProvider.h"
#include "Vi/Source/ViDeviceInfo.h"
#include "Vi/Source/ViChannelInfo.h"

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
    // TODO remove it
    HI_MPI_VB_Exit();

    const HI_U32 channelCount = parent()->viSourceInfo()->viChannelsCount();

    if (channelCount < 1)
        throw std::runtime_error("No channels to process");

    const HI_U32 blockSize = maxPicVbBlkSize();

    if (blockSize < 1)
        throw std::runtime_error("Invalid block size");

    VB_CONF_S stVbConf{};

    stVbConf.u32MaxPoolCnt = DEFAULT_MAX_POOL_COUNT;

    // TODO why * 4 ??
    // < 3 not works

    setPool(stVbConf, 0, blockSize, channelCount * 4);

    // TODO what is it ?
    /* hist buf*/
    setPool(stVbConf, 1, 196 * 4, channelCount * 4);

    if (HI_MPI_VB_SetConf(&stVbConf) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VB_SetConf failed");

    if (HI_MPI_VB_Init() != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VB_Init failed");
}

VideoBuffer::~VideoBuffer() {
    HI_MPI_VB_Exit();
}

HI_U32 VideoBuffer::maxPicVbBlkSize() {
    HI_U32 result = 0, tmp;
    auto& devs = parent()->viSourceInfo()->devices();

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

HI_U32 VideoBuffer::picVbBlkSize(ViChannelInfo *ch) {
    const HI_U32 sysAlignWidth = parent()->sysWidthAlign();
    const TSize imgSize = ch->imgSize();
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
    const HI_U32 bufSize = std::ceil(imgSize.width * imgSize.height * bytesPerPixel);

    // потому что степень для CEILING_2_POWER должна быть кратна 2
    if (sysAlignWidth > 1)
        return CEILING_2_POWER(bufSize, sysAlignWidth);

    return bufSize;
}

}
}
