#ifndef MPP_VI_FAKE_CHANNEL_INFO_H
#define MPP_VI_FAKE_CHANNEL_INFO_H

#include "ChannelInfo.h"

namespace hisilicon::mpp::vi {

class FakeChannelInfo : public ChannelInfo {
  public:
    FakeChannelInfo() {
        TSize size{.width = 352, .height = 240};
        setCapSize(size);
        setImgSize(size);
        setPixelFormat(PIXEL_FORMAT_YUV_SEMIPLANAR_422);
        setScanMode(VI_SCAN_PROGRESSIVE);
        setFps(25);
    }
};

}

#endif // MPP_VI_FAKE_CHANNEL_INFO_H
