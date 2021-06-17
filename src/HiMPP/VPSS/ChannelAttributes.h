#ifndef MPP_VPSS_CHANNEL_ATTRIBUTES_H
#define MPP_VPSS_CHANNEL_ATTRIBUTES_H

#include "HiMPP/ASubsystem/IAttributesBuilder.h"

#include <hi_comm_vpss.h>

namespace hisilicon::mpp::vpss {

class ChannelAttributes : public IAttributesBuilder<VPSS_CHN_ATTR_S> {
  public:
    struct FrameParameters {
        HI_U32 width;
        HI_U32 color;
    };

    MPP_ATTRIBUTE(Frame, FrameParameters);

  protected:
    VPSS_CHN_ATTR_S *buildImpl() override;
};

}

#endif // MPP_VPSS_CHANNEL_ATTRIBUTES_H
