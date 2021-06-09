#ifndef MPP_VBUFFERIZABLE_H
#define MPP_VBUFFERIZABLE_H

#include <mpi_vb.h>

namespace hisilicon::mpp {

// Буфферизируемый объект

class IBufferizable {
public:
    IBufferizable() { }
    virtual ~IBufferizable() { }

    virtual SIZE_S bufferImageSize() = 0;
    virtual PIXEL_FORMAT_E bufferPixelFormat() = 0;
};

}

#endif // MPP_VBUFFERIZABLE_H
