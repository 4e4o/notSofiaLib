#include "DeviceInfo.h"
#include "Misc/Utils.h"

namespace hisilicon::mpp::vi {

DeviceInfo::~DeviceInfo() {
    Utils::clearPtrContainer(m_channels);
}

void DeviceInfo::addChannel(ChannelInfo *c) {
    m_channels.push_back(c);
}

const DeviceInfo::TViChannelsInfo &DeviceInfo::channels() const {
    return m_channels;
}

}
