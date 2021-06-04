#ifndef MPP_VI_SUBSYSTEM_H
#define MPP_VI_SUBSYSTEM_H

#include <vector>

#include "Hisilicon/MPP/MPPChild.h"
#include "Utils/AConfigurator.h"

namespace hisilicon {
namespace mpp {

class ViDevice;

class ViSubsystem : public MPPChild, public AConfigurator {
public:
    ViSubsystem(MPP*);
    ~ViSubsystem();

    ViDevice* addDevice(int id);

private:
    typedef std::vector<ViDevice*> TDevices;
};

}
}

#endif // MPP_VI_SUBSYSTEM_H
