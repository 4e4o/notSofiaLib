#ifndef NVP_SAMPLE_DEFS_H
#define NVP_SAMPLE_DEFS_H

#include "sample_comm.h"

/**
 * Вобщем закоменчу сюда выводы по 7004-lm-v3.
 * Плата имеет hi3520d проц, nvp6134 adc конвертер.
 * на openipc патченная софия грузит проц и лагает стрим иногда, но в целом работает.
 * 1080n = 1080p Width/2 что плохо
 * nvp умеет 1080p не в mix режиме, а вот hi3520d умеет только на dev0->ch0
 * при этом все другие каналы и dev1 не юзабельные.
 * Инфа по hi3520d ограничениям в HiMPP Media Processing Software Development Reference.pdf стр. 121.
 * Инфа по nvp6134 ограничении в mix режиме NVP6134C_Datasheet_release_v01_161012.pdf стр. 25
 */


namespace boards {
namespace lm7004v3 {
class Board;
}
}

namespace hisilicon {
namespace mpp {
namespace vi {
class Channel;

}
}
}

void initAdCompatLayer(boards::lm7004v3::Board *);

struct ChannelInfo {
    //TODO УДАЛИТЬ ЭТО, СДЕЛАТЬ ЧТОБ ЮЗАЮЩИЕ ЭТО ЮЗАЛИ stCapRect/stDestSize/stVencSize
    PIC_SIZE_E sizeType;
};

const ChannelInfo* getChannelInfo(int ch);

hisilicon::mpp::vi::Channel* getViChannel(int ch);

#endif // NVP_SAMPLE_DEFS_H
