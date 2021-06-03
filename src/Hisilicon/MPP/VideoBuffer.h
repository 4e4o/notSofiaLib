#ifndef MPP_VIDEO_BUFFER_H
#define MPP_VIDEO_BUFFER_H

#include "MPPChild.h"

namespace hisilicon {
namespace mpp {

class ViChannelInfo;

class VideoBuffer : public MPPChild {
public:
    VideoBuffer(MPP*);
    ~VideoBuffer();

private:
    HI_U32 maxPicVbBlkSize();
    HI_U32 picVbBlkSize(ViChannelInfo *ch);
};

}
}

#endif // MPP_VIDEO_BUFFER_H
