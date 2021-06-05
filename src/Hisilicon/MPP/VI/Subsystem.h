#ifndef MPP_VI_SUBSYSTEM_H
#define MPP_VI_SUBSYSTEM_H

#include <vector>

#include "Hisilicon/MPP/MPPChild.h"
#include "Utils/Configurator/Configurator.h"

namespace hisilicon {
namespace mpp {
namespace vi {

class Device;

class Subsystem : public MPPChild, public Configurator {
public:
    Subsystem(MPP*);

    Device* addDevice(int id);
};

}
}
}

#endif // MPP_VI_SUBSYSTEM_H
