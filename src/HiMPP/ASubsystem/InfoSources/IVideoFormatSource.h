#ifndef MPP_I_VIDEO_FORMAT_SOURCE_H
#define MPP_I_VIDEO_FORMAT_SOURCE_H

#include "IFrameFormatSource.h"

namespace hisilicon::mpp {

// Интерфейс для классов-источников информации видео формата
// IFrameFormatSource + информация по fps

class IVideoFormatSource : public IFrameFormatSource {
  public:
    virtual HI_U32 fps() const = 0;
};

}

#endif // MPP_I_VIDEO_FORMAT_SOURCE_H
