#ifndef MPP_VPSS_SUBSYSTEM_H
#define MPP_VPSS_SUBSYSTEM_H

#include <vector>

#include "HiMPP/MPPChild.h"
#include "Binder/ConfiguratorBinder.h"

namespace hisilicon::mpp::vpss {

class Group;

class Subsystem : public MPPChild, public ConfiguratorBinder {
public:
    Subsystem(MPP*);

    Group* addGroup(int id);

    const std::vector<Group*>& groups() const;

protected:
    void registerDefaultTypes();
    void addSourceFromVi1by1();

private:
    std::vector<Group*> m_groups;
};

}

#endif // MPP_VPSS_SUBSYSTEM_H
