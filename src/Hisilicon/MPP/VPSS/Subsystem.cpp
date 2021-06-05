#include "Subsystem.h"
#include "Hisilicon/MPP/MPP.h"
#include "Hisilicon/MPP/ElementsFactory.h"
#include "Group.h"

namespace hisilicon {
namespace mpp {
namespace vpss {

Subsystem::Subsystem(MPP* p)
    : MPPChild(p) {
}

bool Subsystem::configureImpl() {
    return Configurator::configureImpl();
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
