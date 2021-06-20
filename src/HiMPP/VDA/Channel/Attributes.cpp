#include "Attributes.h"
#include "HiMPP/ASubsystem/InfoSources/IVideoFormatSource.h"

#include <memory>

namespace hisilicon::mpp::vda {

ChannelAttributes::ChannelAttributes() {
}

VDA_CHN_ATTR_S *ChannelAttributes::buildImpl(IVideoFormatSource *source) {
    std::unique_ptr<VDA_CHN_ATTR_S> result(new VDA_CHN_ATTR_S{});
    const SIZE_S imgSize = source->imgSize();

    // page 716
    // The channel size can be different from the input picture size. However, the width and height
    // of an input picture must be greater than or equal to the channel width and height.
    // 16-byte alignment.

    result->u32Width = FLOOR_2_POWER(imgSize.u32Width, 16);
    result->u32Height = FLOOR_2_POWER(imgSize.u32Height, 16);

    buildForMode(result.get(), source);

    return result.release();
}

}
