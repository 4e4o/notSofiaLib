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

    MPP_ATTRIBUTE(ProfileType, TProfile);
    MPP_ATTRIBUTE(BitrateType, TBitrate);
    MPP_ATTRIBUTE(Bpp, float);
    MPP_ATTRIBUTE(SampleRate, HI_S32);
    MPP_ATTRIBUTE(FrameRate, HI_S32);
    MPP_ATTRIBUTE(NALPrefixSize, HI_S32);

  protected:
    VENC_CHN_ATTR_S *buildImpl(IVideoFormatSource *) override;

  private:
    void onAttach(Channel *, IVideoFormatSource *) override final;
    void onChannelCreated(Channel *, IVideoFormatSource *) override final;
};

}

#endif // MPP_VENC_H264_ATTRIBUTES_BUILDER_H
