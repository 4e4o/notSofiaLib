#include "Subsystem.h"
#include "Hisilicon/MPP/VI/Device.h"

namespace hisilicon {
namespace mpp {
namespace vi {
namespace hi3520dv200 {

Subsystem::Subsystem(MPP * p)
    : mpp::vi::Subsystem(p) {
}

bool Subsystem::configureImpl() {
    // id каналов фиксированы и зависят от хики чипа И режима его работы!
    // в нашем случае 4channel 720p режим для hi3520d v200
    // Hi3520D／Hi3515A／Hi3515C H.264 CODEC Processor Data Sheet.pdf
    // page 616

    // TODO ввести режим работы чипа

    Device* dev0 = addDevice(0);
    Device* dev1 = addDevice(1);

    dev0->addChannel(0, 0, 0);
    dev0->addChannel(2, 0, 1);

    dev1->addChannel(4, 0, 2);
    dev1->addChannel(6, 0, 3);

    return mpp::vi::Subsystem::configureImpl();
}

}
}
}
}
