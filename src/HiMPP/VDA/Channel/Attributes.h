#ifndef MPP_VDA_CHANNEL_ATTRIBUTES_H
#define MPP_VDA_CHANNEL_ATTRIBUTES_H

#include "HiMPP/ASubsystem/IAttributesBuilder.h"

#include <hi_comm_vda.h>

namespace hisilicon::mpp {
class IVideoFormatSource;
}

namespace hisilicon::mpp::vda {

class Channel;

class ChannelAttributes : public
    mpp::IAttributesBuilder<VDA_CHN_ATTR_S, IVideoFormatSource *> {
  public:
    ChannelAttributes();

  private:
    virtual void buildForMode(VDA_CHN_ATTR_S *, IVideoFormatSource *source) = 0;
    VDA_CHN_ATTR_S *buildImpl(IVideoFormatSource *source) override final;
};
}

#endif // MPP_VDA_CHANNEL_ATTRIBUTES_H
