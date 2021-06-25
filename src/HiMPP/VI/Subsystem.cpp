#include "Subsystem.h"
#include "HiMPP/MPP.h"
#include "HiMPP/VI/Source/InfoProvider.h"
#include "HiMPP/VI/Source/IChannelInfo.h"
#include "Device.h"
#include "Channel.h"

namespace hisilicon::mpp::vi {

Subsystem::Subsystem(MPP *p)
    : ASubsystem<Configurator, Device>(p),
      m_infoProvider(p->create<InfoProvider>()) {
    registerDefaultTypes();
    addItemBack(m_infoProvider);
    // конфигурируем его сразу
    m_infoProvider->configure();
}

void Subsystem::registerDefaultTypes() {
    factory()->registerDefaultType<Device, Subsystem *, int>();
    factory()->registerDefaultType<Channel, Device *, const IChannelInfo *, int>();
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
