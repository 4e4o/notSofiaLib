#ifndef MPP_VPSS_SUBSYSTEM_H
#define MPP_VPSS_SUBSYSTEM_H

#include "HiMPP/ASubsystem/ASubsystem.h"
#include "Binder/ConfiguratorBinder.h"

namespace hisilicon::mpp::vpss {

class Group;

class Subsystem : public ASubsystem<ConfiguratorBinder, Group> {
  public:
    Subsystem(MPP *);

    Group *addGroup(int id);

    const std::vector<Group *> &groups() const;

    void addSourceFromVi1by1();

  private:
    void registerDefaultTypes();
};

}

#endif // MPP_VPSS_SUBSYSTEM_H
