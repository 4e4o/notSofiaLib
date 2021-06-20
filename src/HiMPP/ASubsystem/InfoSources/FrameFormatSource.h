#ifndef MPP_FRAME_FORMAT_SOURCE_H
#define MPP_FRAME_FORMAT_SOURCE_H

#include "IFrameFormatSource.h"

namespace hisilicon::mpp {

// Простая реализация

class FrameFormatSource : public IFrameFormatSource {
  public:
    FrameFormatSource(const SIZE_S &s, const PIXEL_FORMAT_E &p)
        : m_size(s), m_pixelFormat(p) { }

    SIZE_S imgSize() const override final {
        return m_size;
    }

    PIXEL_FORMAT_E pixelFormat() const override final {
        return m_pixelFormat;
    }

  private:
    SIZE_S m_size;
    PIXEL_FORMAT_E m_pixelFormat;
};

}

#endif // MPP_FRAME_FORMAT_SOURCE_H
