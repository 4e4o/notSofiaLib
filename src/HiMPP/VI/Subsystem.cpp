#include "Subsystem.h"
#include "HiMPP/MPP.h"
#include "HiMPP/VI/Source/InfoProvider.h"
#include "HiMPP/VI/Source/DeviceInfo.h"
#include "HiMPP/VI/Source/ChannelInfo.h"
#include "Device.h"
#include "Channel.h"

namespace hisilicon::mpp::vi {

Subsystem::Subsystem(MPP *p)
    : ASubsystem<Configurator, Device>(p),
      m_infoProvider(p->create<InfoProvider>()) {
    registerDefaultTypes();
    addItemBack(m_infoProvider);
}

void Subsystem::registerDefaultTypes() {
    factory()->registerType<false>([](Subsystem * p, int id) -> Device* {
        return new Device(p, id);
    });
    factory()->registerType<false>([](Device * d, const ChannelInfo * i,
    int id) -> Channel* {
        return new Channel(d, i, id);
    });
}

Device *Subsystem::addDevice(int id) {
    return addSubItem(this, id);
}

const std::vector<Device *> &Subsystem::devices() const {
    return subItems();
}

vi::InfoProvider *Subsystem::infoProvider() const {
    return m_infoProvider;
}

}
