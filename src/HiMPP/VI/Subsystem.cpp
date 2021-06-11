#include "Subsystem.h"
#include "HiMPP/MPP.h"
#include "HiMPP/VI/Source/InfoProvider.h"
#include "HiMPP/VI/Source/DeviceInfo.h"
#include "HiMPP/VI/Source/ChannelInfo.h"
#include "Device.h"
#include "Channel.h"

namespace hisilicon::mpp::vi {

Subsystem::Subsystem(MPP *p)
    : MPPChild(p),
      m_infoProvider(p->create<InfoProvider>()) {
    registerDefaultTypes();
    addItemBack(m_infoProvider);
}

void Subsystem::registerDefaultTypes() {
    parent()->registerType([](Subsystem * p, int id) -> Device* {
        return new Device(p, id);
    }, false);
    parent()->registerType([](Device * d, const ChannelInfo * i,
    int id) -> Channel* {
        return new Channel(d, i, id);
    }, false);
}

bool Subsystem::configureImpl() {
    // если InfoProvider отконфигурировался
    if (Configurator::configureImpl()) {
        createDevices();
        // конфигурируем девайсы
        return Configurator::configureImpl();
    }

    return false;
}

void Subsystem::createDevices() {
}

Device *Subsystem::addDevice(int id) {
    Device *dev = parent()->create<Device>(this, id);
    addItemBack(dev);
    m_devices.push_back(dev);
    return dev;
}

vi::InfoProvider *Subsystem::infoProvider() const {
    return m_infoProvider;
}

const std::vector<Device *> &Subsystem::devices() const {
    return m_devices;
}

}
