#ifndef MPP_VIDEO_BUFFER_H
#define MPP_VIDEO_BUFFER_H

#include <hi_type.h>

#include "MPPChild.h"
#include "Utils/Configurator/Configurable.h"

namespace hisilicon {
namespace mpp {

namespace vi {
class Channel;
}

class VideoBuffer : public MPPChild, public Configurable {
public:
    VideoBuffer(MPP*);
    ~VideoBuffer();

private:
    bool startImpl();

    HI_U32 maxPicVbBlkSize();
    HI_U32 picVbBlkSize(vi::Channel *ch);
};

}
}

#endif // MPP_VIDEO_BUFFER_H
