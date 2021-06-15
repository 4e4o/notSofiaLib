#ifndef MPP_I_FRAME_FORMAT_SOURCE_H
#define MPP_I_FRAME_FORMAT_SOURCE_H

#include <hi_common.h>

namespace hisilicon::mpp {

// Интерфейс для классов-источников информации формата фрейма

class IFrameFormatSource {
  public:
    virtual ~IFrameFormatSource() { }

    virtual SIZE_S imgSize() const = 0;
    virtual PIXEL_FORMAT_E pixelFormat() const = 0;
};

}

#endif // MPP_I_FRAME_FORMAT_SOURCE_H
