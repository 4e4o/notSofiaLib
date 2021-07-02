#ifndef MPP_VPSS_GROUP_ATTRIBUTES_H
#define MPP_VPSS_GROUP_ATTRIBUTES_H

#include "HiMPP/ASubsystem/Attributes.h"

#include <hi_comm_vpss.h>

namespace hisilicon::mpp {
class IVideoCaptureFormatSource;
}

namespace hisilicon::mpp::vpss {

class Group;

class GroupAttributes : public
    Attributes<Group, VPSS_GRP_ATTR_S> {
  public:
    // HiMPP Media Processing Software Development Reference.pdf
    // page 414
    PROPERTY(DeRing, bool);
    PROPERTY(DeBlocking, bool);
    PROPERTY(ImageEnhancement, bool);
    PROPERTY(NoiseReduction, bool);

  protected:
    VPSS_GRP_ATTR_S *buildAttributesImpl() override;
};

class GroupParameters: public
    Attributes<Group, VPSS_GRP_PARAM_S> {
  public:
    PROPERTY(Luminance, HI_U32);
    PROPERTY(Contrast, HI_U32);
    PROPERTY(DarkEnhance, HI_U32);
    PROPERTY(BrightEnhance, HI_U32);
    PROPERTY(ImageEnhancementStrength, HI_U32);
    PROPERTY(ImageEnhancementSharp, HI_U32);
    PROPERTY(SpatialNoiseReductionStrength, HI_U32);
    PROPERTY(TimeNoiseReductionStrength, HI_U32);
    PROPERTY(DeInterlaceStrength, HI_U32);
    PROPERTY(ChrominanceAmplitude, HI_U32);

    PROPERTY(Crop, RECT_S);

  protected:
    VPSS_GRP_PARAM_S *buildAttributesImpl() override;
    HI_S32 setAttributesImpl(VPSS_GRP_PARAM_S *) override;
    HI_S32 setCrop();
};

}

#endif // MPP_VPSS_GROUP_ATTRIBUTES_H
