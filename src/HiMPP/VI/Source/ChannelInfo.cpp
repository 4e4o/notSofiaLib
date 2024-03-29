#include "ChannelInfo.h"

namespace hisilicon::mpp::vi {

ChannelInfo::ChannelInfo()
    : m_fps(0),
      m_scanMode(VI_SCAN_BUTT),
      m_pixelFormat(PIXEL_FORMAT_BUTT) {
}

void ChannelInfo::setPixelFormat(PIXEL_FORMAT_E pf) {
    m_pixelFormat = pf;
}

PIXEL_FORMAT_E ChannelInfo::pixelFormat() const {
    return m_pixelFormat;
}

TSize ChannelInfo::imgSize() const {
    return m_imgSize;
}

void ChannelInfo::setImgSize(const TSize &imgSize) {
    m_imgSize = imgSize;
}

TSize ChannelInfo::capSize() const {
    return m_capSize;
}

void ChannelInfo::setCapSize(const TSize &capSize) {
    m_capSize = capSize;
}

VI_SCAN_MODE_E ChannelInfo::scanMode() const {
    return m_scanMode;
}

void ChannelInfo::setScanMode(const VI_SCAN_MODE_E &scanMode) {
    m_scanMode = scanMode;
}

HI_U32 ChannelInfo::fps() const {
    return m_fps;
}

void ChannelInfo::setFps(const HI_U32 &fps) {
    m_fps = fps;
}

}
