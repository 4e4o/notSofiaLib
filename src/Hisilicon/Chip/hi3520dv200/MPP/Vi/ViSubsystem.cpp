#include "ViSubsystem.h"
#include "Hisilicon/MPP/VI/ViDevice.h"
#include "Hisilicon/MPP/VI/Source/ViInfoProvider.h"
#include "Hisilicon/MPP/VI/Source/ViDeviceInfo.h"
#include "Hisilicon/MPP/VI/Source/ViChannelInfo.h"
#include "ViChannel.h"

#include <stdexcept>

namespace hisilicon {
namespace mpp {
namespace hi3520dv200 {

ViSubsystem::ViSubsystem(MPP * p)
    : mpp::ViSubsystem(p) {
}

ViSubsystem::~ViSubsystem() {
}

static void addChannel(ViDevice* dev, ViInfoProvider* inf, int chId, int infoDevId, int infoChId) {
    ViChannelInfo* i = inf->findChannelInfo(infoDevId, infoChId);

    // TODO если не нашли то и канал не добавляем
    if (i == NULL)
        throw std::runtime_error("Can't find channel info");

    mpp::ViChannel* ch = dev->addChannel(chId);
    ch->setInfo(i);
}

bool ViSubsystem::configure() {
    ViInfoProvider* inf = parent()->viSourceInfo();

    // id каналов фиксированы и зависят от хики чипа И режима его работы!
    // в нашем случае 4channel 720p режим для hi3520d v200
    // Hi3520D／Hi3515A／Hi3515C H.264 CODEC Processor Data Sheet.pdf
    // page 616

    // TODO ввести режим работы чипа

    ViDevice* dev0 = addDevice(0);
    ViDevice* dev1 = addDevice(1);

    addChannel(dev0, inf, 0, 0, 0);
    addChannel(dev0, inf, 2, 0, 1);

    addChannel(dev1, inf, 4, 0, 2);
    addChannel(dev1, inf, 6, 0, 3);

    return mpp::ViSubsystem::configure();
}

}
}
}
