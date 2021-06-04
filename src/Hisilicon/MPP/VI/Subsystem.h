#ifndef MPP_VI_SUBSYSTEM_H
#define MPP_VI_SUBSYSTEM_H

#include <vector>

#include "Hisilicon/MPP/MPPChild.h"
#include "Utils/AConfigurator.h"

namespace hisilicon {
namespace mpp {
namespace vi {

class Device;

class Subsystem : public MPPChild, public AConfigurator {
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
