#ifndef MPP_VENC_H264_ATTRIBUTES_BUILDER_H
#define MPP_VENC_H264_ATTRIBUTES_BUILDER_H

#include "Attributes.h"

namespace hisilicon::mpp::venc {

class H264Attributes : public ChannelAttributes {
  public:
    H264Attributes();
    ~H264Attributes();

    enum class ProfileType : HI_U32 {
        BASELINE = 0,
        MAIN,
        HIGH
    };

    enum class BitrateType {
        CBR,
        VBR,
        FIXQP
    };

    MPP_ATTRIBUTE(Profile, ProfileType);
    MPP_ATTRIBUTE(Bitrate, BitrateType);
    MPP_ATTRIBUTE(Bpp, float);

  protected:
    VENC_CHN_ATTR_S *buildImpl(IVideoFormatSource *) override;
};

}

#endif // MPP_VENC_H264_ATTRIBUTES_BUILDER_H
