#include "ViSubsystem.h"
#include "Hisilicon/MPP/MPP.h"
#include "Hisilicon/MPP/ElementsFactory.h"
#include "ViDevice.h"

namespace hisilicon {
namespace mpp {

ViSubsystem::ViSubsystem(MPP * p)
    : MPPChild(p) {
}

ViSubsystem::~ViSubsystem() {
}

ViDevice* ViSubsystem::addDevice(int id) {
    ViDevice* dev = parent()->factory()->viDevice(parent(), id);
    addItem(dev);
    return dev;
}

}
}
