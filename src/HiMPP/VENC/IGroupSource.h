#ifndef MPP_GROUP_SOURCE_H
#define MPP_GROUP_SOURCE_H

#include <hi_comm_venc.h>

namespace hisilicon::mpp::venc {

class IGroupSource {
  public:
    IGroupSource() { }
    virtual ~IGroupSource() { }

    virtual SIZE_S imgSize() const = 0;
    virtual bool pal() const = 0;
    virtual PIXEL_FORMAT_E pixelFormat() const = 0;
};

}

#endif // MPP_GROUP_SOURCE_H
