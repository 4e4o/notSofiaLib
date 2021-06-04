#ifndef MPP_VIDEO_BUFFER_H
#define MPP_VIDEO_BUFFER_H

#include "MPPChild.h"
#include "Utils/Configurable/Configurable.h"

namespace hisilicon {
namespace mpp {

namespace vi {
class ChannelInfo;
}

class VideoBuffer : public MPPChild, public Configurable {
public:
    VideoBuffer(MPP*);
    ~VideoBuffer();

private:
    bool configureImpl();

    HI_U32 maxPicVbBlkSize();
    HI_U32 picVbBlkSize(vi::ChannelInfo *ch);
};

}
}

#endif // MPP_VIDEO_BUFFER_H
