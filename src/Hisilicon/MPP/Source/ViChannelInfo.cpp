#include "ViChannelInfo.h"

namespace hisilicon {
namespace mpp {

ViChannelInfo::ViChannelInfo(ViDeviceInfo* d, int id)
    : Holder<ViDeviceInfo *>(d),
      IdHolder(id),
      m_pixelFormat(PIXEL_FORMAT_BUTT) {
}

ViChannelInfo::~ViChannelInfo() {
}

void ViChannelInfo::setPixelFormat(PIXEL_FORMAT_E pf) {
    m_pixelFormat = pf;
}

PIXEL_FORMAT_E ViChannelInfo::pixelFormat() const {
    return m_pixelFormat;
}

TSize ViChannelInfo::imgSize() const {
    return m_imgSize;
}

void ViChannelInfo::setImgSize(const TSize &imgSize) {
    m_imgSize = imgSize;
}

TSize ViChannelInfo::capSize() const{
    return m_capSize;
}

void ViChannelInfo::setCapSize(const TSize &capSize) {
    m_capSize = capSize;
}

}
}
