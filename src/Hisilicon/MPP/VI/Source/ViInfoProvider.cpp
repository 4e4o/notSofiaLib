#include "ViInfoProvider.h"
#include "ViDeviceInfo.h"
#include "ViChannelInfo.h"

namespace hisilicon {
namespace mpp {

ViInfoProvider::ViInfoProvider(const ViInfoProvider::TViDevicesInfo& d)
    : m_devices(d) {
}

ViInfoProvider::~ViInfoProvider() {
}

const ViInfoProvider::TViDevicesInfo& ViInfoProvider::devices() const {
    return m_devices;
}

int ViInfoProvider::viChannelsCount() const {
    int count = 0;

    for (int i = 0 ; i < (int) m_devices.size() ; i++)
        count += m_devices[i]->channels().size();

    return count;
}

ViChannelInfo* ViInfoProvider::findChannelInfo(int devId, int chId) {
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
