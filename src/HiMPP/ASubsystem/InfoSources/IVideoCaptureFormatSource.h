#ifndef MPP_I_VIDEO_CAPTURE_FORMAT_SOURCE_H
#define MPP_I_VIDEO_CAPTURE_FORMAT_SOURCE_H

#include "IVideoFormatSource.h"

namespace hisilicon::mpp {

// Интерфейс для классов-источников информации видео формата захвата
// IVideoFormatSource + информация по размеру картинки захвата

class IVideoCaptureFormatSource : public IVideoFormatSource {
  public:
    virtual SIZE_S destSize() const = 0;
};

}

#endif // MPP_I_VIDEO_CAPTURE_FORMAT_SOURCE_H
