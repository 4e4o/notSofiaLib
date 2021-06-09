#ifndef MPP_VENC_SUBSYSTEM_H
#define MPP_VENC_SUBSYSTEM_H

#include <vector>

#include "HiMPP/MPPChild.h"
#include "HiMPP/VPSS/Binder/ConfiguratorBinder.h"


namespace hisilicon::mpp {
class VBPool;
}

namespace hisilicon::mpp::venc {

class Group;

class Subsystem : public MPPChild, public vpss::ConfiguratorBinder {
public:
    Subsystem(MPP*);

    Group* addGroup(int id);

    // TODO remove it
    Group* group(int id);

    const std::vector<Group*>& groups() const;
    int channelsCount() const;

    enum class PoolAllocationMode {
        PRIVATE_VB_POOL,
        USER_VB_POOL
    };

    void setPoolAllocationMode(PoolAllocationMode m);
    PoolAllocationMode poolAllocationMode() const;

    VBPool* pool() const;

protected:
    bool configureImpl() override;
    void registerDefaultTypes();
    void addSourceFromVpss1by1();

private:
    void createUserPool();
    bool needUserPool();

    std::vector<Group*> m_groups;
    PoolAllocationMode m_poolMode;
    VBPool *m_pool;
};

}

#endif // MPP_VENC_SUBSYSTEM_H
