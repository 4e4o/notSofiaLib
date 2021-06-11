#ifndef MPP_VI_SUBSYSTEM_H
#define MPP_VI_SUBSYSTEM_H

#include <vector>

#include "HiMPP/MPPChild.h"
#include "Misc/Configurator/Configurator.h"

namespace hisilicon::mpp::vi {

class Device;
class InfoProvider;

class Subsystem : public MPPChild, public Configurator {
public:
    Subsystem(MPP*);

    Device* addDevice(int id);

    vi::InfoProvider* infoProvider() const;
    const std::vector<Device*>& devices() const;

protected:
    void registerDefaultTypes();
    bool configureImpl() override final;
    virtual void createDevices();

private:
    std::vector<Device*> m_devices;
    InfoProvider *m_infoProvider;
};

}

#endif // MPP_VI_SUBSYSTEM_H
