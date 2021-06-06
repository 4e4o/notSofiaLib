#include "InfoProvider.h"
#include "DeviceInfo.h"
#include "ChannelInfo.h"

namespace hisilicon::mpp::vi {

InfoProvider::InfoProvider() {
}

InfoProvider::~InfoProvider() {
}

void InfoProvider::setDeviceInfo(const TViDevicesInfo& d) {
    m_devices = d;
}

const InfoProvider::TViDevicesInfo& InfoProvider::devices() const {
    return m_devices;
}

ChannelInfo* InfoProvider::findChannelInfo(int devId, int chId) {
    for (auto& device : devices()) {
        if (device->id() == devId) {
            for (auto& channel : device->channels()) {
                if (channel->id() == chId)
                    return channel;
            }

            return NULL;
        }
    }

    return NULL;
}

}
