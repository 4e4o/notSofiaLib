#ifndef MPP_VI_SUBSYSTEM_H
#define MPP_VI_SUBSYSTEM_H

#include <vector>

#include "Hisilicon/MPP/MPPChild.h"
#include "Utils/Configurable/Configurator.h"

namespace hisilicon {
namespace mpp {
namespace vi {

class Device;

class Subsystem : public MPPChild, public Configurator {
public:
    Subsystem(MPP*);
    ~Subsystem();

    Device* addDevice(int id);

private:
    typedef std::vector<Device*> TDevices;
};

}
}
}

#endif // MPP_VI_SUBSYSTEM_H
