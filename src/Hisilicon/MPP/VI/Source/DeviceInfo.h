#ifndef MPP_VI_DEVICE_INFO_H
#define MPP_VI_DEVICE_INFO_H

#include <vector>

#include "Utils/IdHolder.h"

namespace hisilicon {
namespace mpp {
namespace vi {

class ChannelInfo;

class DeviceInfo : public IdHolder {
public:
    typedef std::vector<ChannelInfo*> TViChannelsInfo;

    DeviceInfo(int id);
    ~DeviceInfo();

    void addChannel(ChannelInfo*);
    const TViChannelsInfo& channels() const;

private:
    TViChannelsInfo m_channels;
};

}
}
}

#endif // MPP_VI_DEVICE_INFO_H
