#include "Subsystem.h"
#include "Group.h"
#include "Channel.h"
#include "ChannelAttributes.h"

namespace hisilicon::mpp::vpss {

Subsystem::Subsystem(MPP *p)
    : ASubsystem(p) {
    registerDefaultTypes();
}

void Subsystem::registerDefaultTypes() {
    factory()->registerDefaultType<Group, Subsystem *, int>();
    factory()->registerDefaultType<Channel, Group *, int>();
}

Group *Subsystem::addGroup(int id) {
    return addSubItem(this, id);
}

const std::vector<Group *> &Subsystem::groups() const {
    return subItems();
}

}
