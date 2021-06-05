#include "DeviceInfo.h"
#include "Utils/Utils.h"

namespace hisilicon {
namespace mpp {
namespace vi {

DeviceInfo::DeviceInfo(int id)
    : IdHolder(id) {
}

DeviceInfo::~DeviceInfo() {
    Utils::clearPtrContainer(m_channels);
}

void DeviceInfo::addChannel(ChannelInfo* c) {
    m_channels.push_back(c);
}

const DeviceInfo::TViChannelsInfo &DeviceInfo::channels() const {
    return m_channels;
}

}
}
}
