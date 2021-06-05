#ifndef MPP_VPSS_SUBSYSTEM_H
#define MPP_VPSS_SUBSYSTEM_H

#include <vector>

#include "Hisilicon/MPP/MPPChild.h"
#include "Utils/Configurator/Configurator.h"

namespace hisilicon {
namespace mpp {
namespace vpss {

class Group;

class Subsystem : public MPPChild, public Configurator {
public:
    Subsystem(MPP*);

    Group* addGroup(int id);

    // TODO remove it
    Group* group(int id);

    const std::vector<Group*>& groups() const;
    int channelsCount() const;

protected:
    bool configureImpl();

private:
    std::vector<Group*> m_groups;
};

}
}
}

#endif // MPP_VPSS_SUBSYSTEM_H
