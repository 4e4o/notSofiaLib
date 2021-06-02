#include <stdio.h>
#include <stdexcept>

#include "ad_cfg.h"
#include "ADC/nvp6134/DriverCommunicator.h"
#include "ADC/nvp6134/Chip.h"
#include "ADC/nvp6134/ViChannel.h"
#include "ADC/nvp6134/VoChannel.h"

using namespace nvp6134;

static DriverCommunicator* g_driverCom;
static Chip* g_chip;

static enum _nvp6134_vi_mode fmtToNvpMode(ViChannel::TVideoFormat df) {
    switch(df) {
    case ViChannel::DF_CVBS_NTSC:
    case ViChannel::DF_CVBS_PAL:
//                return NVP6134_VI_720H;
        //        return NVP6134_VI_1280H; // работает !!!
        //                return NVP6134_VI_1440H;
        // TODO
        // вот тут нипонятно нихуя, почему этот режим выбирается при cvbs захвате?
        return NVP6134_VI_1920H; // !!!!
        //        return NVP6134_VI_1440H;
    case ViChannel::DF_1080P_NTSC:
        return NVP6134_VI_1080P_2530;
    default:
        return NVP6134_VI_1080P_NOVIDEO;
    }
}

static HI_S32 getCaptureRect(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, RECT_S *pstSize) {
    pstSize->s32X = 0;
    pstSize->s32Y = 0;

    // TODO return rect based on nvp vi input channel mode (fmtToNvpMode) !!!!
    // and vdo output mode

    switch (enPicSize)
    {
    case PIC_CIF:
        pstSize->u32Width = D1_WIDTH / 2;
        pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?288:240;
        break;

    case PIC_D1:
        //1920x576i(480)
        // TODO fix it !!!!! wtf ????
        pstSize->u32Width = 1920;//960;//1280;//1920;
        //  pstSize->u32Width = D1_WIDTH;
        pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
        break;
    case PIC_HD1080:
        // TODO / 2 ???
        // because nvp works in 2mux_mix + x_format mode
        pstSize->u32Width = 1920  / 2 ;
        pstSize->u32Height = 1080;
        break;
    default:
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}


SIZE_S getRealPicFormat(struct ChannelInfo* chi) {
    SIZE_S stdFormat;

    if (SAMPLE_COMM_SYS_GetPicSize(chi->norm, chi->sizeType, &(stdFormat)) != HI_SUCCESS)
        throw std::runtime_error("not implemented size type");

    // TODO return format based on nvp x_format mode

    if (stdFormat.u32Width > 960)
        // and (nvp in 2mux_mix mode)
    stdFormat.u32Width = 960;

    return stdFormat;
}

static void setMPPDestForHi3520d(struct ChannelInfo* chi) {
    chi->stDestSize.u32Width = chi->stCapRect.u32Width;
    chi->stDestSize.u32Height = chi->stCapRect.u32Height;

    // HiMPP Media Processing Software Development Reference.pdf
    // page 136/135
    // for hi3520d:
    /** For primary attributes, the width and height
        in stCapRect are static attributes, and others
        are dynamic attributes. For secondary
        attributes, it is meaningless to set stCapRect
        and control the frame rate. Other attributes
        are dynamic attributes.

        stCapRect is based on the source picture. The
        width of stDestSize can be the same as or half
        of the width of stCapRect. The height of
        stDestSize must be half of the height of
        stCapRect in single-field capture mode. In
        dual-field capture mode, the heights must be
        the same.

        In interlaced capture mode, s32Y and
        u32Height must be 4-pixel aligned.
   */

    // т.е получается stDestSize выставляется в зависимости от хики чипа!


    // тут смотрим на формат результирующего изображения
    // если width влазеет в cap::width/2 то юзаем cap::width/2
    // если не влазеет то юзаем cap::width
    // это спец. случай для hi3520d, читай наверху

    HI_U32 halfW = chi->stCapRect.u32Width / 2;
    SIZE_S fmt = getRealPicFormat(chi);

    if (fmt.u32Width <= halfW)
        chi->stDestSize.u32Width = halfW;
}

static PIC_SIZE_E fmtToPicSizeType(enum ViChannel::TVideoFormat df) {

    switch(df) {
    case ViChannel::DF_CVBS_NTSC:
    case ViChannel::DF_CVBS_PAL:
        return PIC_D1;
    case ViChannel::DF_1080P_NTSC:
        return PIC_HD1080;
    default:
        return PIC_BUTT;
    }
}

static void initPicSize(struct ChannelInfo* chi) {
    if (getCaptureRect(chi->norm, chi->sizeType, &(chi->stCapRect)) != HI_SUCCESS) {
        SAMPLE_PRT("1. not implemented size type %i !\n", chi->sizeType);
        return;
    }

    setMPPDestForHi3520d(chi);

    chi->stVencSize = getRealPicFormat(chi);

    // здесь тоже всё понятно
    switch(chi->sizeType) {
    case PIC_D1: {
        chi->scanMode = VI_SCAN_INTERLACED;
        break;
    }
    default: {
        chi->scanMode = VI_SCAN_PROGRESSIVE;
        break;
    }
    }
}

static ChannelInfo g_channelInfo[4];

const ChannelInfo* getChannelInfo(int ch) {
    if ((ch < 0) || (ch > 4))
        throw std::runtime_error("Invalid mpp channel index");

    return &g_channelInfo[ch];
}

void adInit() {
    g_driverCom = new DriverCommunicator(1);
    g_chip = new Chip(g_driverCom, 0);

    auto& c = g_chip->viChannels();

    for (int i = 0 ; i < (int) c.size() ; i++) {
        c[i]->setMode(fmtToNvpMode(c[i]->videoFormat()));

        if (c[i]->videoFormat() == ViChannel::DF_NOT_DETECTED) {
            g_channelInfo[i].hasSignal = 0;
            g_channelInfo[i].norm = VIDEO_ENCODING_MODE_NTSC;
            g_channelInfo[i].sizeType = PIC_CIF;
        } else {
            g_channelInfo[i].hasSignal = 1;
            g_channelInfo[i].norm = c[i]->pal() ? VIDEO_ENCODING_MODE_PAL : VIDEO_ENCODING_MODE_NTSC;
            g_channelInfo[i].sizeType = fmtToPicSizeType(c[i]->videoFormat());
        }

        initPicSize(&g_channelInfo[i]);
    }

    // TODO здесь выбираем режим исходя из формата каналов
    // mix/не mix исходя из пункта 2.11 страница 25
    // mux/не mux исходя из наличия сигналов
    // а выходной формат какой? в случае с mux+mix

    // TODO HiMPP Media Processing Software Development Reference.pdf
    // page 121

    auto& vo = g_chip->voChannels();

    // для NVP6134_OUTMODE_2MUX_MIX режима chid = номеру пары каналов
    // 0 - 0,1
    // !0 - 2,3
    // это в драйвере так сделано
    for (int i = 0 ; i < (int) vo.size() ; i++) {
        vo[i]->setMode(i, NVP6134_OUTMODE_2MUX_MIX);
    }
}
