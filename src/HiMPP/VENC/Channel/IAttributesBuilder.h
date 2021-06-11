#ifndef MPP_VENC_H264_CHANNEL_ATTRIBUTES_BUILDER_H
#define MPP_VENC_H264_CHANNEL_ATTRIBUTES_BUILDER_H

#include <hi_comm_venc.h>

namespace hisilicon::mpp::venc {

class IChannelSource;

class IAttributesBuilder {
  public:
    IAttributesBuilder() { }
    virtual ~IAttributesBuilder() { }

    virtual VENC_CHN_ATTR_S *build(IChannelSource *) = 0;
};

}

#endif // MPP_VENC_H264_CHANNEL_ATTRIBUTES_BUILDER_H
