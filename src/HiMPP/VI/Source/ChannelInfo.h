#ifndef MPP_VI_CHANNEL_INFO_H
#define MPP_VI_CHANNEL_INFO_H

#include <vector>

#include <hi_comm_video.h>
#include <hi_comm_vi.h>

#include "Misc/IdHolder.h"
#include "Misc/Size.h"

namespace hisilicon::mpp::vi {

class DeviceInfo;

class ChannelInfo : public Holder<DeviceInfo *>, public IdHolder {
  public:
    ChannelInfo(DeviceInfo *, int id);
    ~ChannelInfo();

    void setPixelFormat(PIXEL_FORMAT_E);
    PIXEL_FORMAT_E pixelFormat() const;

    TSize imgSize() const;
    void setImgSize(const TSize &imgSize);

    TSize capSize() const;
    void setCapSize(const TSize &capSize);

    VI_SCAN_MODE_E scanMode() const;
    void setScanMode(const VI_SCAN_MODE_E &scanMode);

    HI_U32 fps() const;
    void setFps(const HI_U32 &fps);

  private:
    HI_U32 m_fps;
    VI_SCAN_MODE_E m_scanMode;
    PIXEL_FORMAT_E m_pixelFormat;
    TSize m_imgSize;
    TSize m_capSize;
};

}

#endif // MPP_VI_CHANNEL_INFO_H
