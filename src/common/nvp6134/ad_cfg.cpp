#include <stdio.h>
#include <stdexcept>
#include <string.h>

#include "ad_cfg.h"
#include "Boards/7004_lm_v3/lm7004v3.h"
#include "ADC/nvp6134/Chip.h"
#include "ADC/nvp6134/ViChannel.h"

using namespace nvp6134;

static void setMPPDestForHi3520d(ViChannel* v, struct ChannelInfo* chi) {
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
    TSize imgSize = v->imageSize();

    if (imgSize.width <= halfW)
        chi->stDestSize.u32Width = halfW;
}


static void initPicSize(ViChannel* v, struct ChannelInfo* chi) {
    const TSize cap1 = v->captureSize();
    const TSize imgSize = v->imageSize();

    chi->stCapRect.s32X = 0;
    chi->stCapRect.s32Y = 0;
    chi->stCapRect.u32Height = cap1.height;
    chi->stCapRect.u32Width = cap1.width;

    chi->stVencSize.u32Width = imgSize.width;
    chi->stVencSize.u32Height = imgSize.height;

    setMPPDestForHi3520d(v, chi);

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

static ChannelInfo g_channelInfo[4];

const ChannelInfo* getChannelInfo(int ch) {
    if ((ch < 0) || (ch > 4))
        throw std::runtime_error("Invalid mpp channel index");

    return &g_channelInfo[ch];
}

static boards::lm7004v3::Lm7004v3Board* g_board;

void adInit() {
    g_board = new boards::lm7004v3::Lm7004v3Board();
    g_board->configure();

    {
        auto& c = g_board->nvp()->viChannels();

        for (int i = 0 ; i < (int) c.size() ; i++) {
            if (c[i]->videoFormat() == ViChannel::DF_NOT_DETECTED) {
                g_channelInfo[i].hasSignal = 0;
                g_channelInfo[i].norm = VIDEO_ENCODING_MODE_NTSC;
                g_channelInfo[i].scanMode = VI_SCAN_PROGRESSIVE;
                g_channelInfo[i].sizeType = PIC_CIF;
                g_channelInfo[i].stCapRect.s32X = g_channelInfo[i].stCapRect.s32Y = 0;
                g_channelInfo[i].stCapRect.u32Width = D1_WIDTH / 2;
                g_channelInfo[i].stCapRect.u32Height = 240;
                g_channelInfo[i].stDestSize.u32Height = g_channelInfo[i].stCapRect.u32Height;
                g_channelInfo[i].stDestSize.u32Width = g_channelInfo[i].stCapRect.u32Width;
                g_channelInfo[i].stVencSize = g_channelInfo[i].stDestSize;
            } else {
                g_channelInfo[i].hasSignal = 1;
                g_channelInfo[i].norm = c[i]->pal() ? VIDEO_ENCODING_MODE_PAL : VIDEO_ENCODING_MODE_NTSC;
                g_channelInfo[i].sizeType = fmtToPicSizeType(c[i]->videoFormat());
                initPicSize(c[i].get(), &g_channelInfo[i]);
            }
        }
    }
}
