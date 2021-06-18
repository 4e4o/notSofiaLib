#ifndef MPP_VENC_CHANNEL_ATTRIBUTES_H
#define MPP_VENC_CHANNEL_ATTRIBUTES_H

#include "HiMPP/ASubsystem/IAttributesBuilder.h"

#include <hi_comm_venc.h>

namespace hisilicon::mpp {
class IVideoFormatSource;
}

namespace hisilicon::mpp::venc {

class Channel;

class ChannelAttributes : public
    mpp::IAttributesBuilder<VENC_CHN_ATTR_S, IVideoFormatSource *> {
  public:
    virtual void onAttach(Channel *, IVideoFormatSource *) { }
    virtual void onChannelCreated(Channel *, IVideoFormatSource *) { }
};
}

#endif // MPP_VENC_CHANNEL_ATTRIBUTES_H
