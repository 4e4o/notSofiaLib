#include <stdio.h>
#include <stdexcept>
#include <string.h>

#include "ad_cfg.h"
#include "Boards/7004_lm_v3/Board.h"
#include "ADC/nvp6134/Chip.h"
#include "ADC/nvp6134/ViChannel.h"
#include "Hisilicon/MPP/MPP.h"
#include "Hisilicon/MPP/VI/Subsystem.h"
#include "Hisilicon/MPP/VI/Device.h"
#include "Hisilicon/MPP/VI/Channel.h"

using namespace nvp6134;

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

static boards::lm7004v3::Board* bbb;

hisilicon::mpp::vi::Channel* getViChannel(int ch) {
    if ((ch < 0) || (ch > 4))
        throw std::runtime_error("Invalid mpp channel index");

    int devInd = 0;

    if (ch > 1) {
        devInd = 1;
        ch -= 2;
    }

    hisilicon::mpp::vi::Device* dev = (hisilicon::mpp::vi::Device*)bbb->mpp()->vi()->item(devInd);
    return (hisilicon::mpp::vi::Channel*)dev->item(ch);
}

void initAdCompatLayer(boards::lm7004v3::Board *board) {
    bbb = board;
    auto& c = board->nvp()->viChannels();

    for (int i = 0 ; i < (int) c.size() ; i++) {
        if (!c[i]->formatDetected()) {
            g_channelInfo[i].sizeType = PIC_CIF;
        } else {
            g_channelInfo[i].sizeType = fmtToPicSizeType(c[i]->videoFormat());
        }
    }
}
