#include "ViDeviceInfo.h"
#include "Utils/Utils.h"

namespace hisilicon {
namespace mpp {

ViDeviceInfo::ViDeviceInfo(int id)
    : IdHolder(id) {
}

ViDeviceInfo::~ViDeviceInfo() {
    Utils::clearPtrContainer(m_channels);
}

void ViDeviceInfo::addChannel(ViChannelInfo* c) {
    m_channels.push_back(c);
}

const ViDeviceInfo::TViChannelsInfo &ViDeviceInfo::channels() const {
    return m_channels;
}

}
}
