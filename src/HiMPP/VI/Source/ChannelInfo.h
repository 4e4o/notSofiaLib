#ifndef MPP_VI_CHANNEL_INFO_H
#define MPP_VI_CHANNEL_INFO_H

#include "IChannelInfo.h"

namespace hisilicon::mpp::vi {

class ChannelInfo : public IChannelInfo {
  public:
    ChannelInfo();

  protected:
    PIXEL_FORMAT_E pixelFormat() const override;
    TSize imgSize() const override;
    TSize capSize() const override;
    VI_SCAN_MODE_E scanMode() const override;
    HI_U32 fps() const override;

    void setPixelFormat(PIXEL_FORMAT_E);
    void setImgSize(const TSize &imgSize);
    void setCapSize(const TSize &capSize);
    void setScanMode(const VI_SCAN_MODE_E &scanMode);
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
