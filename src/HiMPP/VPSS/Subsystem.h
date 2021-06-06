#ifndef MPP_VPSS_SUBSYSTEM_H
#define MPP_VPSS_SUBSYSTEM_H

#include <vector>

#include "HiMPP/MPPChild.h"
#include "Utils/Configurator/Configurator.h"

namespace hisilicon::mpp::vpss {

class Group;
class BindSource;
class BindReceiver;

class Subsystem : public MPPChild, public Configurator {
public:
    using MPPChild::MPPChild;

    Group* addGroup(int id);

    // TODO remove it
    Group* group(int id);

    const std::vector<Group*>& groups() const;
    int channelsCount() const;

    void bind(vpss::BindSource*, vpss::BindReceiver*);

protected:
    void addSourceFromVi1by1();

private:
    std::vector<Group*> m_groups;
};

}

#endif // MPP_VPSS_SUBSYSTEM_H
