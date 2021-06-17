#ifndef MPP_VENC_CHANNEL_ATTRIBUTES_H
#define MPP_VENC_CHANNEL_ATTRIBUTES_H

#include "HiMPP/ASubsystem/IAttributesBuilder.h"

#include <hi_comm_venc.h>

namespace hisilicon::mpp {
class IVideoFormatSource;
}

namespace hisilicon::mpp::venc {
class ChannelAttributes : public
    mpp::IAttributesBuilder<VENC_CHN_ATTR_S, IVideoFormatSource *> {};
}

#endif // MPP_VENC_CHANNEL_ATTRIBUTES_H
