#include "Subsystem.h"
#include "HiMPP/MPP.h"
#include "HiMPP/VI/Source/InfoProvider.h"
#include "HiMPP/VI/Source/DeviceInfo.h"
#include "HiMPP/VI/Source/ChannelInfo.h"
#include "Device.h"
#include "Channel.h"

namespace hisilicon::mpp::vi {

Subsystem::Subsystem(MPP* p)
    : MPPChild(p) {
    registerDefaultTypes();
    addItem(p->create<InfoProvider>());
}

void Subsystem::registerDefaultTypes() {
    parent()->registerType([](Subsystem* p, int id) -> Device* {
        return new Device(p, id);
    });
    parent()->registerType([](Device* d, ChannelInfo* i, int id) -> Channel* {
        return new Channel(d, i, id);
    });
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

Device* Subsystem::addDevice(int id) {
    Device* dev = parent()->create<Device>(this, id);
    addItem(dev);
    m_devices.push_back(dev);
    return dev;
}

int Subsystem::channelsCount() const {
    int count = 0;

    for (auto& device : m_devices)
        count += device->channels().size();

    return count;
}

vi::InfoProvider* Subsystem::infoProvider() const {
    return static_cast<vi::InfoProvider*>(item(0));
}

const std::vector<Device*>& Subsystem::devices() const {
    return m_devices;
}

}
