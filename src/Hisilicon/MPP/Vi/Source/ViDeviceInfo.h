#ifndef MPP_VI_DEVICE_INFO_H
#define MPP_VI_DEVICE_INFO_H

#include <vector>

#include "Utils/IdHolder.h"

namespace hisilicon {
namespace mpp {

class ViChannelInfo;

class ViDeviceInfo : public IdHolder {
public:
    typedef std::vector<ViChannelInfo*> TViChannelsInfo;

    ViDeviceInfo(int id);
    ~ViDeviceInfo();

    void addChannel(ViChannelInfo*);
    const TViChannelsInfo& channels() const;

private:
    TViChannelsInfo m_channels;
};

}
}

#endif // MPP_VI_DEVICE_INFO_H
