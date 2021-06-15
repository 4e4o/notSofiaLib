#ifndef MPP_VIDEO_BUFFER_H
#define MPP_VIDEO_BUFFER_H

#include "VBBase.h"

namespace hisilicon::mpp {

class VideoBuffer : public VBBase {
  public:
    using VBBase::VBBase;
    ~VideoBuffer();

  protected:
    bool configureImpl() override;

  private:
    virtual HI_U32 getBlocksCount(const HI_U32 &);

    HI_U32 maxViBlkSize(HI_U32 &channelsCount);
};

}

#endif // MPP_VIDEO_BUFFER_H
