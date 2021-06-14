#include "Subsystem.h"
#include "Group.h"
#include "HiMPP/VB/VBPool.h"

namespace hisilicon::mpp::venc {

Subsystem::Subsystem(MPP *p)
    : ASubsystem(p),
      m_poolMode(PoolAllocationMode::PRIVATE_VB_POOL),
      m_pool(nullptr) {
    registerDefaultTypes();
}

Subsystem::~Subsystem() {
}

void Subsystem::registerDefaultTypes() {
    factory()->registerType([](Subsystem * p, int id) -> Group* {
        return new Group(p, id);
    }, false);
    factory()->registerType([](Group * g, int id) -> Channel* {
        return new Channel(g, id);
    }, false);
}

Group *Subsystem::addGroup(int id) {
    return addSubItem(this, id);
}

const std::vector<Group *> &Subsystem::groups() const {
    return subItems();
}

VBPool *Subsystem::pool() const {
    return m_pool;
}

bool Subsystem::configureImpl() {
    createUserPool();
    return ConfiguratorBinder::configureImpl();
}

void Subsystem::createUserPool() {
    if (!needUserPool())
        return;

    m_pool = new VBPool(parent());
    m_pool->initForVenc();
    addItemFront(m_pool);
}

bool Subsystem::needUserPool() {
    for (auto &group : groups()) {
        for (auto &channel : group->channels()) {
            if (channel->needUserPool())
                return true;
        }
    }

    return false;
}

void Subsystem::setPoolAllocationMode(PoolAllocationMode m) {
    // режим работы пула
    // если h264e.ko загружен с H264eVBSource == 1
    // то должен быть равен PRIVATE_VB_POOL
    // если загружен с H264eVBSource == 2 параметром
    // то == USER_VB_POOL
    // по дефолту == PRIVATE_VB_POOL
    // HiMPP Media Processing Software Development Reference.pdf
    // стр. 511
    m_poolMode = m;
}

Subsystem::PoolAllocationMode Subsystem::poolAllocationMode() const {
    return m_poolMode;
}

}
