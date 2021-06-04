#include "Subsystem.h"
#include "Hisilicon/MPP/MPP.h"
#include "Hisilicon/MPP/ElementsFactory.h"
#include "Device.h"

namespace hisilicon {
namespace mpp {
namespace vi {

Subsystem::Subsystem(MPP * p)
    : MPPChild(p) {
}

Subsystem::~Subsystem() {
}

Device* Subsystem::addDevice(int id) {
    Device* dev = parent()->factory()->viDevice(parent(), id);
    addItem(dev);
    return dev;
}

}
}
}
