#ifndef MPP_VENC_H264_ATTRIBUTES_BUILDER_H
#define MPP_VENC_H264_ATTRIBUTES_BUILDER_H

#include "Attributes.h"

namespace hisilicon::mpp::venc {

class H264Attributes : public ChannelAttributes {
  public:
    H264Attributes();

    enum class TProfile : HI_U32 {
        BASELINE = 0,
        MAIN,
        HIGH
    };

    enum class TBitrate {
        CBR,
        VBR,
        FIXQP
    };

    PROPERTY(ProfileType, TProfile);
    PROPERTY(BitrateType, TBitrate);
    PROPERTY(Bpp, float);
    PROPERTY(SampleRate, HI_S32);
    PROPERTY(FrameRate, HI_S32);
    PROPERTY(NALPrefixSize, HI_S32);

  protected:
    VENC_CHN_ATTR_S *buildAttributesImpl() override;

  private:
    void onChannelCreated() override final;
    void onAttached() override final;
};

}

#endif // MPP_VENC_H264_ATTRIBUTES_BUILDER_H
