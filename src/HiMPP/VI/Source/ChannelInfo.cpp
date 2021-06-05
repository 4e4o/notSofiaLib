#include "ChannelInfo.h"

namespace hisilicon {
namespace mpp {
namespace vi {

ChannelInfo::ChannelInfo(DeviceInfo* d, int id)
    : Holder<DeviceInfo *>(d),
      IdHolder(id),
      m_pal(false),
      m_scanMode(VI_SCAN_BUTT),
      m_pixelFormat(PIXEL_FORMAT_BUTT) {
}

ChannelInfo::~ChannelInfo() {
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

TSize ChannelInfo::capSize() const{
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

bool ChannelInfo::pal() const {
    return m_pal;
}

void ChannelInfo::setPal(bool pal) {
    m_pal = pal;
}

}
}
}
