#ifndef MPP_VI_INFO_PROVIDER_H
#define MPP_VI_INFO_PROVIDER_H

#include <vector>
#include <memory>

namespace hisilicon {
namespace mpp {
namespace vi {

class DeviceInfo;
class ChannelInfo;

// Задача класса -
// Передавать инфу из источника по Vi devices
// и Vi channels в MPP

class InfoProvider {
public:
    typedef std::vector<DeviceInfo*> TViDevicesInfo;

    InfoProvider(const TViDevicesInfo&);
    ~InfoProvider();

    const TViDevicesInfo& devices() const;
    int viChannelsCount() const;

    ChannelInfo* findChannelInfo(int devId, int chId);

private:
    TViDevicesInfo m_devices;
};

}
}
}

#endif // MPP_VI_INFO_PROVIDER_H
