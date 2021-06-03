#ifndef MPP_VI_INFO_PROVIDER_H
#define MPP_VI_INFO_PROVIDER_H

#include <vector>
#include <memory>

namespace hisilicon {
namespace mpp {

class ViDeviceInfo;
class ViChannelInfo;

// Задача класса -
// Передавать инфу из источника по Vi devices
// и Vi channels в MPP

class ViInfoProvider {
public:
    typedef std::vector<ViDeviceInfo*> TViDevicesInfo;

    ViInfoProvider(const TViDevicesInfo&);
    ~ViInfoProvider();

    const TViDevicesInfo& devices() const;
    int viChannelsCount() const;

    ViChannelInfo* findChannelInfo(int devId, int chId);

private:
    TViDevicesInfo m_devices;
};

}
}

#endif // MPP_VI_INFO_PROVIDER_H
