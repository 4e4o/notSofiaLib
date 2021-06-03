#ifndef MPP_VI_CHANNEL_INFO_H
#define MPP_VI_CHANNEL_INFO_H

#include <vector>

#include <hi_comm_video.h>

#include "Utils/IdHolder.h"
#include "Utils/Size.h"

namespace hisilicon {
namespace mpp {

class ViDeviceInfo;

class ViChannelInfo : public Holder<ViDeviceInfo*>, public IdHolder {
public:
    ViChannelInfo(ViDeviceInfo*, int id);
    ~ViChannelInfo();

    void setPixelFormat(PIXEL_FORMAT_E);
    PIXEL_FORMAT_E pixelFormat() const;

    TSize imgSize() const;
    void setImgSize(const TSize &imgSize);

    TSize capSize() const;
    void setCapSize(const TSize &capSize);

private:
    PIXEL_FORMAT_E m_pixelFormat;
    TSize m_imgSize;
    TSize m_capSize;
};

}
}

#endif // MPP_VI_CHANNEL_INFO_H
