#include "Subsystem.h"
#include "Group.h"
#include "Channel.h"

namespace hisilicon::mpp::vpss {

Subsystem::Subsystem(MPP *p)
    : ASubsystem(p) {
    registerDefaultTypes();
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

}
