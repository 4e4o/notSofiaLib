#include "ChannelInfo.h"
#include "ADC/nvp6134/ViChannel.h"

namespace boards::nvp6134::mpp::vi {

using ::nvp6134::ViChannel;

static PIXEL_FORMAT_E toMppPixelFormat(ViChannel *ch) {
    switch (ch->pixelFormat()) {
    case ViChannel::OutPixelFormat::YUV_422:
        return PIXEL_FORMAT_YUV_SEMIPLANAR_422;
        break;
    default:
        throw std::runtime_error("Invalid pixel format from nvp");
    }
}

static VI_SCAN_MODE_E toMppScanMode(ViChannel *ch) {
    if (ch->scanMode() == ViChannel::ScanMode::PROGRESSIVE)
        return VI_SCAN_PROGRESSIVE;

    return VI_SCAN_INTERLACED;
}

ChannelInfo::ChannelInfo(::nvp6134::ViChannel *c)
    : Holder< ::nvp6134::ViChannel *>(c) {
    setPixelFormat(toMppPixelFormat(value()));
    setImgSize(value()->imageSize());
    setCapSize(value()->captureSize());
    setScanMode(toMppScanMode(value()));
    setFps(value()->fps());
}

::nvp6134::ViChannel *ChannelInfo::viChannel() const {
    return value();
}

}
