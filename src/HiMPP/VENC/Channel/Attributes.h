#ifndef MPP_VENC_CHANNEL_ATTRIBUTES_H
#define MPP_VENC_CHANNEL_ATTRIBUTES_H

#include "HiMPP/ASubsystem/Attributes.h"

#include <hi_comm_venc.h>

namespace hisilicon::mpp::venc {

class Channel;

class ChannelAttributes : public
    Attributes<Channel, VENC_CHN_ATTR_S> {
  public:
    virtual void onChannelCreated() { }
};
}

#endif // MPP_VENC_CHANNEL_ATTRIBUTES_H
