#include "MotionAttributes.h"
#include "HiMPP/ASubsystem/InfoSources/IVideoFormatSource.h"

namespace hisilicon::mpp::vda {

MotionAttributes::MotionAttributes() {
}

void MotionAttributes::buildForMode(VDA_CHN_ATTR_S *attr,
                                    IVideoFormatSource *source) {

    attr->enWorkMode = VDA_WORK_MODE_MD;
    VDA_MD_ATTR_S &mdAttr = attr->unAttr.stMdAttr;
    mdAttr.enVdaAlg = VDA_ALG_BG;
    mdAttr.enMbSize = VDA_MB_16PIXEL;
    mdAttr.enMbSadBits = VDA_MB_SAD_8BIT;
    mdAttr.enRefMode = VDA_REF_MODE_DYNAMIC;
    mdAttr.u32MdBufNum = 8;
    // TODO
    mdAttr.u32VdaIntvl = source->fps() / 4;
    mdAttr.u32BgUpSrcWgt = 128;
    mdAttr.u32SadTh = 100;
    mdAttr.u32ObjNumMax = 128;
}


}
