#ifndef MPP_VENC_SUBSYSTEM_H
#define MPP_VENC_SUBSYSTEM_H

#include <vector>

#include "HiMPP/MPPChild.h"
#include "Utils/Configurator/Configurator.h"

namespace hisilicon::mpp::venc {

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
    void registerDefaultTypes();

private:
    std::vector<Group*> m_groups;
};

}

#endif // MPP_VENC_SUBSYSTEM_H
