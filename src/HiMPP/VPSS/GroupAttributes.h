#ifndef MPP_VPSS_GROUP_ATTRIBUTES_H
#define MPP_VPSS_GROUP_ATTRIBUTES_H

#include "HiMPP/ASubsystem/IAttributesBuilder.h"

#include <hi_comm_vpss.h>

namespace hisilicon::mpp {
class IVideoCaptureFormatSource;
}

namespace hisilicon::mpp::vpss {

class GroupAttributes : public
    IAttributesBuilder<VPSS_GRP_ATTR_S, IVideoCaptureFormatSource *> {
  public:
    // HiMPP Media Processing Software Development Reference.pdf
    // page 414
    MPP_ATTRIBUTE(DeRing, bool);
    MPP_ATTRIBUTE(DeBlocking, bool);
    MPP_ATTRIBUTE(ImageEnhancement, bool);
    MPP_ATTRIBUTE(NoiseReduction, bool);

  protected:
    VPSS_GRP_ATTR_S *buildImpl(IVideoCaptureFormatSource *) override;
};

class GroupParameters: public
    IAttributesBuilder<VPSS_GRP_PARAM_S> {
  public:
    MPP_ATTRIBUTE(Luminance, HI_U32);
    MPP_ATTRIBUTE(Contrast, HI_U32);
    MPP_ATTRIBUTE(DarkEnhance, HI_U32);
    MPP_ATTRIBUTE(BrightEnhance, HI_U32);
    MPP_ATTRIBUTE(ImageEnhancementStrength, HI_U32);
    MPP_ATTRIBUTE(ImageEnhancementSharp, HI_U32);
    MPP_ATTRIBUTE(SpatialNoiseReductionStrength, HI_U32);
    MPP_ATTRIBUTE(TimeNoiseReductionStrength, HI_U32);
    MPP_ATTRIBUTE(DeInterlaceStrength, HI_U32);
    MPP_ATTRIBUTE(ChrominanceAmplitude, HI_U32);

  protected:
    VPSS_GRP_PARAM_S *buildImpl() override;
};

}

#endif // MPP_VPSS_GROUP_ATTRIBUTES_H
