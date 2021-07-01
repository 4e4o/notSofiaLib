#include "GroupAttributes.h"
#include "Group.h"
#include "HiMPP/ASubsystem/InfoSources/IVideoCaptureFormatSource.h"

#include <mpi_vpss.h>

namespace hisilicon::mpp::vpss {

VPSS_GRP_ATTR_S *GroupAttributes::buildAttributesImpl() {
    const IVideoCaptureFormatSource *source = parent()->source();
    std::unique_ptr<VPSS_GRP_ATTR_S> result(new VPSS_GRP_ATTR_S{});

    result->enPixFmt = source->pixelFormat();

    const SIZE_S size = source->destSize();
    result->u32MaxW = size.u32Width;
    result->u32MaxH = size.u32Height;

    result->bDbEn = toHIBool(get<DeBlocking>());
    result->bDrEn = toHIBool(get<DeRing>());
    result->bIeEn = toHIBool(get<ImageEnhancement>());
    result->bNrEn = toHIBool(get<NoiseReduction>());

    return result.release();
}

VPSS_GRP_PARAM_S *GroupParameters::buildAttributesImpl() {
    std::unique_ptr<VPSS_GRP_PARAM_S> result(new VPSS_GRP_PARAM_S{});

    result->u32Luminance = get<Luminance>();
    result->u32Contrast = get<Contrast>();
    result->u32DarkEnhance = get<DarkEnhance>();
    result->u32BrightEnhance = get<BrightEnhance>();
    result->u32IeStrength = get<ImageEnhancementStrength>();
    result->u32IeSharp = get<ImageEnhancementSharp>();
    result->u32SfStrength = get<SpatialNoiseReductionStrength>();
    result->u32TfStrength = get<TimeNoiseReductionStrength>();
    result->u32DiStrength = get<DeInterlaceStrength>();
    result->u32ChromaRange = get<ChrominanceAmplitude>();

    return result.release();
}

HI_S32 GroupParameters::setAttributesImpl(VPSS_GRP_PARAM_S *p) {
    return HI_MPI_VPSS_SetGrpParam(parent()->id(), p);
}

}
