#include "Subsystem.h"
#include "HiMPP/MPP.h"
#include "HiMPP/VI/Subsystem.h"
#include "HiMPP/VI/Device.h"
#include "HiMPP/VI/Channel.h"
#include "Binder/Binder.h"
#include "Group.h"
#include "Channel.h"

namespace hisilicon::mpp::vpss {

Subsystem::Subsystem(MPP* p)
    : MPPChild(p) {
    p->registerType([](Subsystem* p, int id) -> Group* {
        return new Group(p, id);
    });
    p->registerType([](Group* g, int id) -> Channel* {
        return new Channel(g, id);
    });
}

// Добавляет vpss группу на каждый канал vi.
// в каждой группе по одному каналу.
// Должен быть вызван когда девайсы и каналы vi уже добавлены.
void Subsystem::addSourceFromVi1by1() {
    int groupId = 0;

    for (auto& device : parent()->vi()->devices()) {
        for (auto& channel : device->channels()) {
            Group* group = addGroup(groupId++);
            group->setAttributes(channel);
            group->addChannel(0);
            bind(channel, group);
        }
    }
}

void Subsystem::bind(vpss::BindSource* s, vpss::BindReceiver* r) {
    addItem(new vpss::Binder(s, r));
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
