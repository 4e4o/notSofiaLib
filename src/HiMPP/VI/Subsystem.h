#ifndef MPP_VI_SUBSYSTEM_H
#define MPP_VI_SUBSYSTEM_H

#include "HiMPP/ASubsystem/ASubsystem.h"
#include "Misc/Configurator/Configurator.h"

namespace hisilicon::mpp::vi {

class Device;
class InfoProvider;

class Subsystem : public ASubsystem<Configurator, Device> {
  public:
    Subsystem(MPP *);

    Device *addDevice(int id);

    vi::InfoProvider *infoProvider() const;
    const std::vector<Device *> &devices() const;

  protected:
    void registerDefaultTypes();

  private:
    InfoProvider *m_infoProvider;
};

}

#endif // MPP_VI_SUBSYSTEM_H
