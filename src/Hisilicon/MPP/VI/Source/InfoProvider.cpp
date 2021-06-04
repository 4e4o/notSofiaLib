#include "InfoProvider.h"
#include "DeviceInfo.h"
#include "ChannelInfo.h"

namespace hisilicon {
namespace mpp {
namespace vi {

InfoProvider::InfoProvider(const InfoProvider::TViDevicesInfo& d)
    : m_devices(d) {
}

InfoProvider::~InfoProvider() {
}

const InfoProvider::TViDevicesInfo& InfoProvider::devices() const {
    return m_devices;
}

int InfoProvider::viChannelsCount() const {
    int count = 0;

    for (int i = 0 ; i < (int) m_devices.size() ; i++)
        count += m_devices[i]->channels().size();

    return count;
}

ChannelInfo* InfoProvider::findChannelInfo(int devId, int chId) {
    auto& devs = devices();

    for (int i = 0 ; i < (int)devs.size() ; i++) {
        auto dev = devs[i];

        if (dev->id() == devId) {
            auto& channels = dev->channels();

            for (int j = 0 ; j < (int)channels.size() ; j++) {
                auto channel = channels[j];

                if (channel->id() == chId)
                    return channel;
            }

            return NULL;
        }
    }

    return NULL;
}

}
}
}
