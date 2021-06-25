#ifndef MPP_VI_ICHANNEL_INFO_H
#define MPP_VI_ICHANNEL_INFO_H

#include <hi_comm_vi.h>

#include "Misc/Size.h"

namespace hisilicon::mpp::vi {

class IChannelInfo {
  public:
    virtual ~IChannelInfo() { }

    virtual PIXEL_FORMAT_E pixelFormat() const = 0;
    virtual TSize imgSize() const = 0;
    virtual TSize capSize() const = 0;
    virtual VI_SCAN_MODE_E scanMode() const = 0;
    virtual HI_U32 fps() const = 0;
};

}

#endif // MPP_VI_ICHANNEL_INFO_H
