#ifndef MPP_VENC_SUBSYSTEM_H
#define MPP_VENC_SUBSYSTEM_H

#include <vector>
#include <thread>

#include "HiMPP/MPPChild.h"
#include "HiMPP/VPSS/Binder/ConfiguratorBinder.h"


namespace hisilicon::mpp {
class VBPool;
}

namespace hisilicon::mpp::venc {

class Group;
class StreamLoop;

class Subsystem : public MPPChild, public vpss::ConfiguratorBinder {
public:
    Subsystem(MPP*);
    ~Subsystem();

    Group* addGroup(int id);

    const std::vector<Group*>& groups() const;

    enum class PoolAllocationMode {
        PRIVATE_VB_POOL,
        USER_VB_POOL
    };

    void setPoolAllocationMode(PoolAllocationMode m);
    PoolAllocationMode poolAllocationMode() const;

    VBPool* pool() const;

    void setStreamLoopsCount(int streamLoopsCount);
    StreamLoop* getLoopForChannel();

    void run();
    void stop();

protected:
    bool configureImpl() override;
    void registerDefaultTypes();
    void addSourceFromVpss1by1();

private:
    void createUserPool();
    bool needUserPool();
    void createStreamLoops();
    void joinStreamThreads();

    std::vector<Group*> m_groups;
    PoolAllocationMode m_poolMode;
    VBPool *m_pool;
    std::vector<StreamLoop*> m_streamLoops;
    int m_streamLoopsCount;
    std::vector<std::thread*> m_threads;
    int m_channelLoopIndex;
};

}

#endif // MPP_VENC_SUBSYSTEM_H
