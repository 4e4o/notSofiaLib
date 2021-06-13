#ifndef MPP_VENC_H264_ATTRIBUTES_BUILDER_H
#define MPP_VENC_H264_ATTRIBUTES_BUILDER_H

#include "Channel.h"
#include "IAttributesBuilder.h"

namespace hisilicon::mpp::venc {

class H264AttributesBuilder : public IAttributesBuilder {
  public:
    H264AttributesBuilder();
    ~H264AttributesBuilder();

    enum class Profile : HI_U32 {
        BASELINE = 0,
        MAIN,
        HIGH
    };

    void setBitrateType(const Channel::BitrateType &bitrateType);
    void setProfile(const Profile &profile);
    void setBpp(float bpp);

    VENC_CHN_ATTR_S *build(IChannelSource *) override final;

  private:
    Channel::BitrateType m_bitrateType;
    Profile m_profile;
    float m_bpp;
};

}

#endif // MPP_VENC_H264_ATTRIBUTES_BUILDER_H
