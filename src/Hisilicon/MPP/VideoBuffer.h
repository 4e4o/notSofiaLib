#ifndef MPP_VIDEO_BUFFER_H
#define MPP_VIDEO_BUFFER_H

#include "MPPChild.h"

namespace hisilicon {
namespace mpp {

namespace vi {
class ChannelInfo;
}

class VideoBuffer : public MPPChild {
public:
    VideoBuffer(MPP*);
    ~VideoBuffer();

private:
    HI_U32 maxPicVbBlkSize();
    HI_U32 picVbBlkSize(vi::ChannelInfo *ch);
};

}
}

#endif // MPP_VIDEO_BUFFER_H
