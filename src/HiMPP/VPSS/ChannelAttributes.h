#ifndef MPP_VPSS_CHANNEL_ATTRIBUTES_H
#define MPP_VPSS_CHANNEL_ATTRIBUTES_H

#include "HiMPP/ASubsystem/Attributes.h"

#include <hi_comm_vpss.h>

namespace hisilicon::mpp::vpss {

class Channel;

class ChannelAttributes : public Attributes<Channel, VPSS_CHN_ATTR_S> {
  public:
    struct FrameParameters {
        HI_U32 width;
        HI_U32 color;
    };

    PROPERTY(Frame, FrameParameters);

  protected:
    VPSS_CHN_ATTR_S *buildAttributesImpl() override;
    HI_S32 setAttributesImpl(VPSS_CHN_ATTR_S *) override;
};

}

#endif // MPP_VPSS_CHANNEL_ATTRIBUTES_H
