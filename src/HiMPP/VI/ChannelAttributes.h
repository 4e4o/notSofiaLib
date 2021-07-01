#ifndef MPP_VI_CHANNEL_ATTRIBUTES_H
#define MPP_VI_CHANNEL_ATTRIBUTES_H

#include "HiMPP/ASubsystem/Attributes.h"

#include <hi_comm_vi.h>

namespace hisilicon::mpp::vi {

class Channel;

class ChannelAttributes : public
    Attributes<Channel, VI_CHN_ATTR_S> {
  public:
    PROPERTY(FrameRate, HI_S32);
    PROPERTY(Mirror, bool);
    PROPERTY(Flip, bool);

  protected:
    VI_CHN_ATTR_S *buildAttributesImpl() override;
    HI_S32 setAttributesImpl(VI_CHN_ATTR_S *) override;
};

}

#endif // MPP_VI_CHANNEL_ATTRIBUTES_H
