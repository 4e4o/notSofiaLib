#include "Subsystem.h"
#include "HiMPP/MPP.h"
#include "Group.h"
#include "Channel/Channel.h"
#include "Channel/H264AttributesBuilder.h"
#include "HiMPP/VPSS/Subsystem.h"
#include "HiMPP/VPSS/Group.h"
#include "HiMPP/VPSS/Channel.h"
#include "HiMPP/VB/VBPool.h"

namespace hisilicon::mpp::venc {

Subsystem::Subsystem(MPP* p)
    : MPPChild(p),
      m_poolMode(PoolAllocationMode::PRIVATE_VB_POOL),
      m_pool(nullptr) {
    registerDefaultTypes();
}

void Subsystem::registerDefaultTypes() {
    parent()->registerType([](Subsystem* p, int id) -> Group* {
        return new Group(p, id);
    });
    parent()->registerType([](Group* g, int id) -> Channel* {
        return new Channel(g, id);
    });
}

VBPool* Subsystem::pool() const {
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
    for (auto& group : m_groups) {
        for (auto& channel : group->channels()) {
            if (channel->needUserPool())
                return true;
        }
    }

    return false;
}

// Добавляет venc группу на каждую группу vpss.
// в каждой группе по одному каналу.
// Должен быть вызван когда группы и каналы vpss уже добавлены.
void Subsystem::addSourceFromVpss1by1() {
    int id = 0;

    for (auto& vpss_group : parent()->vpss()->groups()) {
        Group* group = addGroup(id);
        Channel* channel = group->addChannel(id++);
        channel->setSource(vpss_group);
        H264AttributesBuilder* ab = new H264AttributesBuilder();
        channel->setAttributesBuilder(ab);
        bind(vpss_group, group);
    }
}

Group* Subsystem::addGroup(int id) {
    Group* group = parent()->create<Group>(this, id);
    addItem(group);
    m_groups.push_back(group);
    return group;
}

int Subsystem::channelsCount() const {
    int count = 0;

    for (auto& group : m_groups)
        count += group->channels().size();

    return count;
}

const std::vector<Group*>& Subsystem::groups() const {
    return m_groups;
}

Group* Subsystem::group(int id) {
    return m_groups[id];
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
