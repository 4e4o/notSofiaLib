#include "H264Attributes.h"
#include "HiMPP/ASubsystem/InfoSources/IVideoFormatSource.h"
#include "Channel.h"

#include <cmath>
#include <iostream>
#include <memory>

#include <mpi_venc.h>

namespace hisilicon::mpp::venc {

#define DEFAULT_SAMPLE_RATE         90000
#define DEFAULT_BPP                 0.03f
#define DEFAULT_NAL_PREFIX_SIZE     4
#define DEFAULT_PROFILE             TProfile::BASELINE
#define DEFAULT_BITRATE_TYPE        TBitrate::VBR

// TODO мб более точный метод высчитывания битрейта для h264 есть
// Зависит от того чего мы хотим, битрейт влияет на качество и объем трафа
// Надо наверно какие-то две функции на выбор выдавать, большее качество
// или меньший траф

// https://www.sri.com/wp-content/uploads/pdf/3_07_h264_format_bitrate_quality_tradeoff_study.pdf
static constexpr HI_U32 bitrate(float bpp, const SIZE_S &size,
                                const HI_U32 &fps,
                                float scaleFactor) {
    return (bpp * size.u32Width * size.u32Height * scaleFactor * fps) / 1000;
}

H264Attributes::H264Attributes() {
    set<NALPrefixSize>(DEFAULT_NAL_PREFIX_SIZE);
    set<SampleRate>(DEFAULT_SAMPLE_RATE);
    set<Bpp>(DEFAULT_BPP);
    set<BitrateType>(DEFAULT_BITRATE_TYPE);
    set<ProfileType>(DEFAULT_PROFILE);
}

void H264Attributes::onAttached() {
    const HI_U32 source_fps = parent()->source()->fps();

    if (!contains<FrameRate>())
        set<FrameRate>(source_fps);
}

VENC_CHN_ATTR_S *H264Attributes::buildAttributesImpl() {
    const IVideoFormatSource *source = parent()->source();
    std::unique_ptr<VENC_CHN_ATTR_S> result(new VENC_CHN_ATTR_S{});
    VENC_ATTR_H264_S &stH264Attr = result->stVeAttr.stAttrH264e;

    const HI_U32 source_fps = source->fps();
    const SIZE_S picSize = source->imgSize();
    const float scaleFactor = source->pixelFormat() ==
                              PIXEL_FORMAT_YUV_SEMIPLANAR_422 ? 2.0f : 1.5f;
    const float bpp = get<Bpp>();
    const HI_U32 target_fps = get<FrameRate>();
    const HI_U32 bit_rate = bitrate(bpp, picSize, target_fps, scaleFactor);
    const TProfile profile = get<ProfileType>();
    const TBitrate bitrateType = get<BitrateType>();

    std::cout << "H264Attributes bitrate " << bit_rate << " kib/s, "
              << (((bit_rate * 1000) / 8) * 60) / (1024 * 1024) << " Mb/min" << std::endl;

    result->stVeAttr.enType = PT_H264;

    stH264Attr.u32MaxPicWidth = picSize.u32Width;
    stH264Attr.u32MaxPicHeight = picSize.u32Height;
    stH264Attr.u32PicWidth = picSize.u32Width;
    stH264Attr.u32PicHeight = picSize.u32Height;

    // HiMPP Media Processing Software Development Reference.pdf
    // page 627
    const HI_U32 bufSize = std::ceil(picSize.u32Width * picSize.u32Height * 1.5f);
    stH264Attr.u32BufSize = CEILING_2_POWER(bufSize, 64);

    stH264Attr.u32Profile = static_cast<HI_U32>(profile);
    stH264Attr.bByFrame = HI_TRUE;
    stH264Attr.bField = HI_FALSE;
    stH264Attr.bMainStream = HI_TRUE;
    stH264Attr.u32Priority = 0;
    stH264Attr.bVIField = HI_FALSE;

    if (bitrateType == TBitrate::CBR) {
        VENC_ATTR_H264_CBR_S &stH264Cbr = result->stRcAttr.stAttrH264Cbr;
        result->stRcAttr.enRcMode = VENC_RC_MODE_H264CBRv2;
        stH264Cbr.u32Gop = target_fps;
        stH264Cbr.u32StatTime = 1;
        stH264Cbr.u32ViFrmRate = source_fps;
        stH264Cbr.fr32TargetFrmRate = target_fps;
        stH264Cbr.u32FluctuateLevel = 0;
        stH264Cbr.u32BitRate = bit_rate;
    } else if (bitrateType == TBitrate::FIXQP) {
        VENC_ATTR_H264_FIXQP_S &stH264FixQp = result->stRcAttr.stAttrH264FixQp;
        result->stRcAttr.enRcMode = VENC_RC_MODE_H264FIXQP;
        stH264FixQp.u32Gop = target_fps;
        stH264FixQp.u32ViFrmRate = source_fps;
        stH264FixQp.fr32TargetFrmRate = target_fps;
        stH264FixQp.u32IQp = 20;
        stH264FixQp.u32PQp = 23;
    } else if (bitrateType == TBitrate::VBR) {
        VENC_ATTR_H264_VBR_S &stH264Vbr = result->stRcAttr.stAttrH264Vbr;
        result->stRcAttr.enRcMode = VENC_RC_MODE_H264VBRv2;
        stH264Vbr.u32Gop = target_fps;
        stH264Vbr.u32StatTime = 1;
        stH264Vbr.u32ViFrmRate = source_fps;
        stH264Vbr.fr32TargetFrmRate = target_fps;
        stH264Vbr.u32MinQp = 10;
        stH264Vbr.u32MaxQp = 51;
        stH264Vbr.u32MaxBitRate = bit_rate;
    } else
        throw std::runtime_error("Unknown bitrate type");

    return result.release();
}

void H264Attributes::onChannelCreated() {
    std::unique_ptr<VENC_PARAM_H264_VUI_S> attrs(new VENC_PARAM_H264_VUI_S{});

    attrs->timing_info_present_flag = 1;
    attrs->fixed_frame_rate_flag = 1;
    attrs->time_scale = get<SampleRate>();
    attrs->num_units_in_tick = attrs->time_scale / (2 * get<FrameRate>());

    if (HI_MPI_VENC_SetH264Vui(parent()->id(), attrs.get()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_SetH264Vui failed");
}

}
