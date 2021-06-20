#ifndef MPP_VI_CHANNEL_INFO_H
#define MPP_VI_CHANNEL_INFO_H

#include <hi_comm_vi.h>

#include "Misc/Size.h"

namespace hisilicon::mpp::vi {

class ChannelInfo {
  public:
    ChannelInfo();
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
