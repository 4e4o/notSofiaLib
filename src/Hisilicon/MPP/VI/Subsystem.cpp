#include "Subsystem.h"
#include "Hisilicon/MPP/MPP.h"
#include "Hisilicon/MPP/ElementsFactory.h"
#include "Hisilicon/MPP/VI/Source/InfoProvider.h"
#include "Hisilicon/MPP/VI/Source/DeviceInfo.h"
#include "Hisilicon/MPP/VI/Source/ChannelInfo.h"
#include "Device.h"

namespace hisilicon {
namespace mpp {
namespace vi {

Subsystem::Subsystem(MPP* p)
    : MPPChild(p) {
    addItem(p->factory()->viInfoProvider());
}

bool Subsystem::configureImpl() {
    // если viInfoProvider отконфигурировался
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
    Device* dev = parent()->factory()->viDevice(this, id);
    addItem(dev);
    m_devices.push_back(dev);
    return dev;
}

int Subsystem::channelsCount() const {
    int count = 0;

    for (int i = 0 ; i < (int) m_devices.size() ; i++)
        count += m_devices[i]->channels().size();

    return count;
}

vi::InfoProvider* Subsystem::infoProvider() const {
    return static_cast<vi::InfoProvider*>(item(0));
}

const std::vector<Device*>& Subsystem::devices() const {
    return m_devices;
}

Device* Subsystem::device(int id) {
    return m_devices[id];
}

}
}
}
