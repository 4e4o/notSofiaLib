#include "Subsystem.h"
#include "HiMPP/MPP.h"
#include "HiMPP/VI/Subsystem.h"
#include "HiMPP/VI/Device.h"
#include "HiMPP/VI/Channel.h"
#include "Binder/Binder.h"
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

// Добавляет vpss группу на каждый канал vi.
// в каждой группе по одному каналу.
// Должен быть вызван когда девайсы и каналы vi уже добавлены.
void Subsystem::addSourceFromVi1by1() {
    int groupId = 0;

    for (auto &device : parent()->vi()->devices()) {
        for (auto &channel : device->channels()) {
            Group *group = addGroup(groupId++);
            group->setSource(channel);
            group->addChannel(0);
            bind(channel, group);
        }
    }
}

}
