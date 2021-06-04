#include "Subsystem.h"
#include "Hisilicon/MPP/VI/Device.h"
#include "Hisilicon/MPP/VI/Source/InfoProvider.h"
#include "Hisilicon/MPP/VI/Source/DeviceInfo.h"
#include "Hisilicon/MPP/VI/Source/ChannelInfo.h"
#include "Channel.h"

#include <stdexcept>

namespace hisilicon {
namespace mpp {
namespace vi {
namespace hi3520dv200 {

Subsystem::Subsystem(MPP * p)
    : mpp::vi::Subsystem(p) {
}

Subsystem::~Subsystem() {
}

static void addChannel(Device* dev, InfoProvider* inf, int chId, int infoDevId, int infoChId) {
    ChannelInfo* i = inf->findChannelInfo(infoDevId, infoChId);

    // TODO если не нашли то и канал не добавляем
    if (i == NULL)
        throw std::runtime_error("Can't find channel info");

    mpp::vi::Channel* ch = dev->addChannel(chId);
    ch->setInfo(i);
}

bool Subsystem::configure() {
    InfoProvider* inf = parent()->viSourceInfo();

    // id каналов фиксированы и зависят от хики чипа И режима его работы!
    // в нашем случае 4channel 720p режим для hi3520d v200
    // Hi3520D／Hi3515A／Hi3515C H.264 CODEC Processor Data Sheet.pdf
    // page 616

    // TODO ввести режим работы чипа

    Device* dev0 = addDevice(0);
    Device* dev1 = addDevice(1);

    addChannel(dev0, inf, 0, 0, 0);
    addChannel(dev0, inf, 2, 0, 1);

    addChannel(dev1, inf, 4, 0, 2);
    addChannel(dev1, inf, 6, 0, 3);

    return mpp::vi::Subsystem::configure();
}

}
}
}
}
