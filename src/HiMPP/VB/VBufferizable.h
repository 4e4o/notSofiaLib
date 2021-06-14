#ifndef MPP_VBUFFERIZABLE_H
#define MPP_VBUFFERIZABLE_H

#include <hi_common.h>

namespace hisilicon::mpp {

// Буфферизируемый объект

class IVBufferizable {
  public:
    IVBufferizable() { }
    virtual ~IVBufferizable() { }

    virtual SIZE_S vbImageSize() = 0;
    virtual PIXEL_FORMAT_E vbPixelFormat() = 0;
};

}

#endif // MPP_VBUFFERIZABLE_H
