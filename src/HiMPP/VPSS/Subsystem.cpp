#include "Subsystem.h"
#include "HiMPP/MPP.h"
#include "HiMPP/ElementsFactory.h"
#include "HiMPP/VI/Subsystem.h"
#include "HiMPP/VI/Device.h"
#include "HiMPP/VI/Channel.h"
#include "Binder/Binder.h"
#include "Group.h"

namespace hisilicon {
namespace mpp {
namespace vpss {

Subsystem::Subsystem(MPP* p)
    : MPPChild(p) {
}

// Добавляет vpss группу на каждый канал vi.
// в каждой группе по одному каналу.
// Должен быть вызван когда девайсы и каналы vi уже добавлены.
void Subsystem::addSourceFromVi1by1() {
    auto& devs = parent()->vi()->devices();
    int groupId = 0;

    for (int i = 0 ; i < (int) devs.size() ; i++) {
        auto& chnls = devs[i]->channels();

        for (int j = 0 ; j < (int) chnls.size() ; j++) {
            Group* group = addGroup(groupId++);
            group->addChannel(0);
            bind(chnls[j], group);
        }
    }
}

void Subsystem::bind(vpss::BindSource* s, vpss::BindReceiver* r) {
    addItem(new vpss::Binder(s, r));
}

Group* Subsystem::addGroup(int id) {
    Group* group = parent()->factory()->vpssGroup(this, id);
    addItem(group);
    m_groups.push_back(group);
    return group;
}

int Subsystem::channelsCount() const {
    int count = 0;

    for (int i = 0 ; i < (int) m_groups.size() ; i++)
        count += m_groups[i]->channels().size();

    return count;
}

const std::vector<Group*>& Subsystem::groups() const {
    return m_groups;
}

Group* Subsystem::group(int id) {
    return m_groups[id];
}

}
}
}
