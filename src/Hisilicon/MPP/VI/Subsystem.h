#ifndef MPP_VI_SUBSYSTEM_H
#define MPP_VI_SUBSYSTEM_H

#include <vector>

#include "Hisilicon/MPP/MPPChild.h"
#include "Utils/Configurator/Configurator.h"

namespace hisilicon {
namespace mpp {
namespace vi {

class Device;
class InfoProvider;

class Subsystem : public MPPChild, public Configurator {
public:
    Subsystem(MPP*);

    // TODO remove it
    vi::InfoProvider* infoProvider() const;

    Device* addDevice(int id);

    // TODO remove it
    Device* device(int id);

    const std::vector<Device*>& devices() const;
    int channelsCount() const;

protected:
    bool configureImpl();
    virtual void createDevices();

private:
    std::vector<Device*> m_devices;
};

}
}
}

#endif // MPP_VI_SUBSYSTEM_H
