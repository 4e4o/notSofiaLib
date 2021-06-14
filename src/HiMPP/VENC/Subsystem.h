#ifndef MPP_VENC_SUBSYSTEM_H
#define MPP_VENC_SUBSYSTEM_H

#include "HiMPP/ASubsystem/ASubsystem.h"
#include "HiMPP/VPSS/Binder/ConfiguratorBinder.h"

namespace hisilicon::mpp {
class VBPool;
}

namespace hisilicon::mpp::venc {

class Group;

class Subsystem : public ASubsystem<vpss::ConfiguratorBinder, Group> {
  public:
    Subsystem(MPP *);
    ~Subsystem();

    Group *addGroup(int id);

    const std::vector<Group *> &groups() const;

    enum class PoolAllocationMode {
        PRIVATE_VB_POOL,
        USER_VB_POOL
    };

    void setPoolAllocationMode(PoolAllocationMode m);
    PoolAllocationMode poolAllocationMode() const;

    VBPool *pool() const;

  protected:
    bool configureImpl() override;
    void registerDefaultTypes();

  private:
    void createUserPool();
    bool needUserPool();

    PoolAllocationMode m_poolMode;
    VBPool *m_pool;
};

}

#endif // MPP_VENC_SUBSYSTEM_H
