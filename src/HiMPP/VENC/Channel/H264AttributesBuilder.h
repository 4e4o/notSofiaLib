#ifndef MPP_VENC_H264_ATTRIBUTES_BUILDER_H
#define MPP_VENC_H264_ATTRIBUTES_BUILDER_H

#include "Channel.h"
#include "IAttributesBuilder.h"

namespace hisilicon::mpp::venc {

class H264AttributesBuilder : public IAttributesBuilder {
public:
    H264AttributesBuilder();
    ~H264AttributesBuilder();

    VENC_CHN_ATTR_S* build(IChannelSource*) override final;

    void setBitrateType(const Channel::BitrateType &bitrateType);

private:
    Channel::BitrateType m_bitrateType;
};

}

#endif // MPP_VENC_H264_ATTRIBUTES_BUILDER_H
