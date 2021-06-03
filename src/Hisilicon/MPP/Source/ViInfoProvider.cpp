#include "ViInfoProvider.h"
#include "ViDeviceInfo.h"

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

}
}
