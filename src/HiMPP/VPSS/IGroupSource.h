#ifndef MPP_VPSS_GROUP_SOURCE_H
#define MPP_VPSS_GROUP_SOURCE_H

#include <hi_common.h>

namespace hisilicon::mpp::vpss {

class IGroupSource {
  public:
    IGroupSource() { }
    virtual ~IGroupSource() { }

    virtual SIZE_S destSize() const = 0;
    virtual SIZE_S imgSize() const = 0;
    virtual HI_U32 fps() const = 0;
    virtual PIXEL_FORMAT_E pixelFormat() const = 0;
};

}

#endif // MPP_VPSS_GROUP_SOURCE_H
