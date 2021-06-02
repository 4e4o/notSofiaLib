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
        //        return NVP6134_VI_960H;
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

static HI_S32 SAMPLE_COMM_SYS_GetPicSizeMYs(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, SIZE_S *pstSize)
{
    switch (enPicSize)
    {
    case PIC_CIF:
        pstSize->u32Width = D1_WIDTH / 2;
        pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?288:240;
        break;

    case PIC_D1:
        //1920x576i(480)
        // TODO fix it !!!!! wtf ????
        pstSize->u32Width = 1920 / 2 ;//960;//1280;//1920;
        pstSize->u32Height = 480;//576;
        //  pstSize->u32Width = D1_WIDTH;
        //pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
        break;
    case PIC_HD1080:
        // TODO / 2 ???
        pstSize->u32Width = 1920  / 2 ;
        pstSize->u32Height = 1080;
        break;
    default:
        return HI_FAILURE;
    }
    return HI_SUCCESS;
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

SIZE_S limitedByHI3520DChSize(SIZE_S s) {
    SIZE_S res = s;

    if(res.u32Width > 960)
        res.u32Width = 960;

    return res;
}

static void initPicSize(struct ChannelInfo* chi) {
    if (SAMPLE_COMM_SYS_GetPicSizeMYs(chi->norm, chi->sizeType, &(chi->stDestSize)) != HI_SUCCESS) {
        SAMPLE_PRT("1. not implemented size type %i !\n", chi->sizeType);
        return;
    }

    if (SAMPLE_COMM_SYS_GetPicSize(chi->norm, chi->sizeType, &(chi->stPicSize)) != HI_SUCCESS) {
        SAMPLE_PRT("2. not implemented size type %i !\n", chi->sizeType);
        return;
    }

    chi->stPicSize = limitedByHI3520DChSize(chi->stPicSize);

    // TODO fix it !!!
    // ну здесь понятно мы ограничиваем макс возможным сайзом
    /*    if (chi->stDestSize.u32Width > 960) {
        chi->stDestSize.u32Width = 960;
    }

    if (chi->stDestSize.u32Height > 1080) {
        chi->stDestSize.u32Height = 1080;
    }
*/

    chi->stCapRect.s32X = 0;
    chi->stCapRect.s32Y = 0;
    chi->stCapRect.u32Width = chi->stDestSize.u32Width;
    chi->stCapRect.u32Height = chi->stDestSize.u32Height;

    // здесь тоже всё понятно
    //
    switch(chi->sizeType) {
    case PIC_D1: {
        chi->scanMode = VI_SCAN_INTERLACED;
        // TODO whyyyyy????
        // MPP page 136
        chi->stCapRect.u32Width *= 2;
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
