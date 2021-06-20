#include "Subsystem.h"
#include "Channel/Channel.h"
#include "Channel/Motion/MotionChannel.h"

namespace hisilicon::mpp::vda {

Subsystem::Subsystem(MPP *p)
    : ASubsystem(p) {
    registerDefaultTypes();
}

Subsystem::~Subsystem() {
}

void Subsystem::registerDefaultTypes() {
    factory()->registerDefaultType<MotionChannel, Subsystem *, int>();
}

MotionChannel *Subsystem::addMotionChannel(int id) {
    return addDerivedSubItem<MotionChannel>(this, id);
}

const std::vector<Channel *> &Subsystem::channels() const {
    return subItems();
}

}
