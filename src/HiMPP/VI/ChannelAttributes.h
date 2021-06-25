#ifndef MPP_VI_CHANNEL_ATTRIBUTES_H
#define MPP_VI_CHANNEL_ATTRIBUTES_H

#include "HiMPP/ASubsystem/IAttributesBuilder.h"

#include <hi_comm_vi.h>

namespace hisilicon::mpp::vi {

class Channel;
class IChannelInfo;

class ChannelAttributes : public
    IAttributesBuilder<VI_CHN_ATTR_S, Channel *, const IChannelInfo *> {
  public:
    ChannelAttributes();

    MPP_ATTRIBUTE(FrameRate, HI_S32);
    MPP_ATTRIBUTE(Mirror, bool);
    MPP_ATTRIBUTE(Flip, bool);

  protected:
    VI_CHN_ATTR_S *buildImpl(Channel *, const IChannelInfo *) override;
};

}

#endif // MPP_VI_CHANNEL_ATTRIBUTES_H
