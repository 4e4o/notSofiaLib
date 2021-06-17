#include "ChannelAttributes.h"

#include <memory>

namespace hisilicon::mpp::vpss {

VPSS_CHN_ATTR_S *ChannelAttributes::buildImpl() {
    std::unique_ptr<VPSS_CHN_ATTR_S> result(new VPSS_CHN_ATTR_S{});

    if (contains<Frame>()) {
        const auto &info = get<Frame>();
        result->bFrameEn = HI_TRUE;

        for (int i = VPSS_FRAME_WORK_LEFT ; i <= VPSS_FRAME_WORK_TOP ; i++ ) {
            result->stFrame.u32Color[i] = info.color;
            result->stFrame.u32Width[i] = info.width;
        }
    }

    return result.release();
}

}
