#ifndef MPP_VIDEO_BUFFER_H
#define MPP_VIDEO_BUFFER_H

#include "VBBase.h"

namespace hisilicon::mpp {

class IBufferizable;

class VideoBuffer : public VBBase {
public:
    using VBBase::VBBase;
    ~VideoBuffer();

private:
    bool startImpl() override final;

    HI_U32 maxViBlkSize();
};

}

#endif // MPP_VIDEO_BUFFER_H
