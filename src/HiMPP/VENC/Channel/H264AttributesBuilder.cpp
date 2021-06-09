#include "H264AttributesBuilder.h"
#include "IChannelSource.h"

#include <cmath>

namespace hisilicon::mpp::venc {

#define BPP 0.12f

// https://www.sri.com/wp-content/uploads/pdf/3_07_h264_format_bitrate_quality_tradeoff_study.pdf
static constexpr HI_U32 bitrate(const SIZE_S& size, const HI_U32& fps, float scaleFactor) {
    return (BPP * size.u32Width * size.u32Height * scaleFactor * fps) / 1000;
}

H264AttributesBuilder::H264AttributesBuilder() :
    m_bitrateType(Channel::BitrateType::VBR) {
}

VENC_CHN_ATTR_S* H264AttributesBuilder::build(IChannelSource* source) {

    std::unique_ptr<VENC_CHN_ATTR_S> result(new VENC_CHN_ATTR_S());
    VENC_ATTR_H264_S& stH264Attr = result->stVeAttr.stAttrH264e;

    const SIZE_S picSize = source->imgSize();
    const HI_U32 fps = source->pal() ? 25 : 30;
    const float scaleFactor = source->pixelFormat() == PIXEL_FORMAT_YUV_SEMIPLANAR_422 ? 2.0f : 1.5f;

    result->stVeAttr.enType = PT_H264;

    stH264Attr.u32MaxPicWidth = picSize.u32Width;
    stH264Attr.u32MaxPicHeight = picSize.u32Height;
    stH264Attr.u32PicWidth = picSize.u32Width;
    stH264Attr.u32PicHeight = picSize.u32Height;

    // HiMPP Media Processing Software Development Reference.pdf
    // page 627
    const HI_U32 bufSize = std::ceil(picSize.u32Width * picSize.u32Height * 1.5f);
    stH264Attr.u32BufSize = CEILING_2_POWER(bufSize, 64);

    stH264Attr.u32Profile = 0; /*0: baseline; 1:MP; 2:HP   ? */
    stH264Attr.bByFrame = HI_TRUE; /*get stream mode is slice mode or frame mode?*/
    stH264Attr.bField = HI_FALSE; /* surpport frame code only for hi3516, bfield = HI_FALSE */
    stH264Attr.bMainStream = HI_TRUE; /* surpport main stream only for hi3516, bMainStream = HI_TRUE */
    stH264Attr.u32Priority = 0; /*channels precedence level. invalidate for hi3516*/
    stH264Attr.bVIField = HI_FALSE; /*the sign of the VI picture is field or frame. Invalidate for hi3516*/

    if(m_bitrateType == Channel::BitrateType::CBR) {
        VENC_ATTR_H264_CBR_S& stH264Cbr = result->stRcAttr.stAttrH264Cbr;

        result->stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
        stH264Cbr.u32Gop = fps;
        stH264Cbr.u32StatTime = 1; /* stream rate statics time(s) */
        stH264Cbr.u32ViFrmRate = fps; /* input (vi) frame rate */
        stH264Cbr.fr32TargetFrmRate = fps; /* target frame rate */
        stH264Cbr.u32FluctuateLevel = 0; /* average bit rate */

        // page 652
        // Average bit rate, in kbit/s
        // Value range: [2, 40960]
        stH264Cbr.u32BitRate = bitrate(picSize, fps, scaleFactor);
    } else if (m_bitrateType == Channel::BitrateType::FIXQP) {
        VENC_ATTR_H264_FIXQP_S& stH264FixQp = result->stRcAttr.stAttrH264FixQp;
        result->stRcAttr.enRcMode = VENC_RC_MODE_H264FIXQP;
        stH264FixQp.u32Gop = fps;
        stH264FixQp.u32ViFrmRate = fps;
        stH264FixQp.fr32TargetFrmRate = fps;
        stH264FixQp.u32IQp = 20;
        stH264FixQp.u32PQp = 23;
    } else if (m_bitrateType == Channel::BitrateType::VBR) {
        VENC_ATTR_H264_VBR_S& stH264Vbr = result->stRcAttr.stAttrH264Vbr;
        result->stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
        stH264Vbr.u32Gop = fps;
        stH264Vbr.u32StatTime = 1;
        stH264Vbr.u32ViFrmRate = fps;
        stH264Vbr.fr32TargetFrmRate = fps;
        stH264Vbr.u32MinQp = 10;
        stH264Vbr.u32MaxQp = 40;
        stH264Vbr.u32MaxBitRate = 2 * bitrate(picSize, fps, scaleFactor);
    } else
        throw std::runtime_error("Unknown bitrate type");

    return result.release();
}

void H264AttributesBuilder::setBitrateType(const Channel::BitrateType &bitrateType) {
    m_bitrateType = bitrateType;
}

H264AttributesBuilder::~H264AttributesBuilder() {
}

}