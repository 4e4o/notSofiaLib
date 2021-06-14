#ifndef MPP_VENC_GROUP_SOURCE_H
#define MPP_VENC_GROUP_SOURCE_H

#include <hi_comm_venc.h>

namespace hisilicon::mpp::venc {

class IGroupSource {
  public:
    IGroupSource() { }
    virtual ~IGroupSource() { }

    virtual SIZE_S imgSize() const = 0;
    virtual HI_U32 fps() const = 0;
    virtual PIXEL_FORMAT_E pixelFormat() const = 0;
};

}

#endif // MPP_VENC_GROUP_SOURCE_H
