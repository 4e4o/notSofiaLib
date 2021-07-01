#include "ChannelAttributes.h"
#include "Channel.h"
#include "Source/IChannelInfo.h"
#include "HiMPP/Misc/Utils.h"

#include <mpi_vi.h>

namespace hisilicon::mpp::vi {

VI_CHN_ATTR_S *ChannelAttributes::buildAttributesImpl() {
    const IChannelInfo *info = parent()->source();
    std::unique_ptr<VI_CHN_ATTR_S> result(new VI_CHN_ATTR_S{});

    // TODO от чего это зависит ?
    result->enCapSel = VI_CAPSEL_BOTH;

    if (contains<FrameRate>()) {
        result->s32SrcFrameRate = info->fps();
        result->s32FrameRate = get<FrameRate>();

        if (result->s32FrameRate > result->s32SrcFrameRate)
            throw std::runtime_error("Target frame rate must be <= source");
    } else {
        result->s32SrcFrameRate = -1;
        result->s32FrameRate = -1;
    }

    result->enPixFormat = info->pixelFormat();
    result->stCapRect = Utils::toMppRect(info->capSize());
    result->stDestSize = parent()->destSize();

    result->bMirror = toHIBool(get<Mirror>());
    result->bFlip = toHIBool(get<Flip>());

    return result.release();
}

HI_S32 ChannelAttributes::setAttributesImpl(VI_CHN_ATTR_S *a) {
    const IChannelInfo *info = parent()->source();
    const HI_S32 scanModeRes = HI_MPI_VI_SetChnScanMode(parent()->id(),
                               info->scanMode());

    if (scanModeRes != HI_SUCCESS)
        return HI_SUCCESS;

    return HI_MPI_VI_SetChnAttr(parent()->id(), a);
}

}
