#include "H264Attributes.h"
#include "HiMPP/ASubsystem/InfoSources/IVideoFormatSource.h"

#include <cmath>
#include <iostream>
#include <memory>

namespace hisilicon::mpp::venc {

#define DEFAULT_BPP 0.03f

// TODO мб более точный метод высчитывания битрейта для h264 есть

// https://www.sri.com/wp-content/uploads/pdf/3_07_h264_format_bitrate_quality_tradeoff_study.pdf
static constexpr HI_U32 bitrate(float bpp, const SIZE_S &size,
                                const HI_U32 &fps,
                                float scaleFactor) {
    return (bpp * size.u32Width * size.u32Height * scaleFactor * fps) / 1000;
}

H264Attributes::H264Attributes() {
    set<Bpp>(DEFAULT_BPP);
    set<Bitrate>(BitrateType::VBR);
    set<Profile>(ProfileType::HIGH);
}

VENC_CHN_ATTR_S *H264Attributes::buildImpl(IVideoFormatSource *source) {
    std::unique_ptr<VENC_CHN_ATTR_S> result(new VENC_CHN_ATTR_S{});
    VENC_ATTR_H264_S &stH264Attr = result->stVeAttr.stAttrH264e;

    const SIZE_S picSize = source->imgSize();
    const HI_U32 fps = source->fps();
    const float scaleFactor = source->pixelFormat() ==
                              PIXEL_FORMAT_YUV_SEMIPLANAR_422 ? 2.0f : 1.5f;
    const float bpp = get<Bpp>();
    const HI_U32 bit_rate = bitrate(bpp, picSize, fps, scaleFactor);
    const ProfileType profile = get<Profile>();
    const BitrateType bitrateType = get<Bitrate>();

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

    /*0: baseline; 1:MP; 2:HP */
    stH264Attr.u32Profile = static_cast<HI_U32>(profile);
    /*get stream mode is slice mode or frame mode?*/
    stH264Attr.bByFrame = HI_TRUE;
    /* surpport frame code only for hi3516, bfield = HI_FALSE */
    stH264Attr.bField = HI_FALSE;
    /* surpport main stream only for hi3516, bMainStream = HI_TRUE */
    stH264Attr.bMainStream = HI_TRUE;
    /*channels precedence level. invalidate for hi3516*/
    stH264Attr.u32Priority = 0;
    /*the sign of the VI picture is field or frame. Invalidate for hi3516*/
    stH264Attr.bVIField = HI_FALSE;

    if (bitrateType == BitrateType::CBR) {
        VENC_ATTR_H264_CBR_S &stH264Cbr = result->stRcAttr.stAttrH264Cbr;

        result->stRcAttr.enRcMode = VENC_RC_MODE_H264CBRv2;
        stH264Cbr.u32Gop = fps;
        /* stream rate statics time(s) */
        stH264Cbr.u32StatTime = 1;
        /* input (vi) frame rate */
        stH264Cbr.u32ViFrmRate = fps;
        /* target frame rate */
        stH264Cbr.fr32TargetFrmRate = fps;
        /* average bit rate */
        stH264Cbr.u32FluctuateLevel = 0;

        // page 652
        // Average bit rate, in kbit/s
        // Value range: [2, 40960]
        stH264Cbr.u32BitRate = bit_rate;
    } else if (bitrateType == BitrateType::FIXQP) {
        VENC_ATTR_H264_FIXQP_S &stH264FixQp = result->stRcAttr.stAttrH264FixQp;
        result->stRcAttr.enRcMode = VENC_RC_MODE_H264FIXQP;
        stH264FixQp.u32Gop = fps;
        stH264FixQp.u32ViFrmRate = fps;
        stH264FixQp.fr32TargetFrmRate = fps;
        stH264FixQp.u32IQp = 20;
        stH264FixQp.u32PQp = 23;
    } else if (bitrateType == BitrateType::VBR) {
        VENC_ATTR_H264_VBR_S &stH264Vbr = result->stRcAttr.stAttrH264Vbr;
        result->stRcAttr.enRcMode = VENC_RC_MODE_H264VBRv2;
        stH264Vbr.u32Gop = fps;
        stH264Vbr.u32StatTime = 1;
        stH264Vbr.u32ViFrmRate = fps;
        stH264Vbr.fr32TargetFrmRate = fps;
        stH264Vbr.u32MinQp = 10;
        stH264Vbr.u32MaxQp = 51;
        stH264Vbr.u32MaxBitRate = bit_rate;
    } else
        throw std::runtime_error("Unknown bitrate type");

    return result.release();
}

H264Attributes::~H264Attributes() {
}

}
