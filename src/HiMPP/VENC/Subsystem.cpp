#include "Subsystem.h"
#include "HiMPP/MPP.h"
#include "HiMPP/VI/Subsystem.h"
#include "HiMPP/VI/Device.h"
#include "HiMPP/VI/Channel.h"
#include "Group.h"
#include "Channel.h"

namespace hisilicon::mpp::venc {

Subsystem::Subsystem(MPP* p)
    : MPPChild(p) {
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

}
