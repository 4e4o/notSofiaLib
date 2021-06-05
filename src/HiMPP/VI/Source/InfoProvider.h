#ifndef MPP_VI_INFO_PROVIDER_H
#define MPP_VI_INFO_PROVIDER_H

#include <vector>
#include <memory>

#include "Utils/Configurator/Configurable.h"

namespace hisilicon {
namespace mpp {
namespace vi {

class DeviceInfo;
class ChannelInfo;

// Задача класса -
// Передавать инфу из источника по Vi devices
// и Vi channels в MPP

class InfoProvider : public Configurable {
public:
    typedef std::vector<DeviceInfo*> TViDevicesInfo;

    InfoProvider();
    ~InfoProvider();

    const TViDevicesInfo& devices() const;

    ChannelInfo* findChannelInfo(int devId, int chId);

protected:
    void setDeviceInfo(const TViDevicesInfo&);

private:
    TViDevicesInfo m_devices;
};

}
}
}

#endif // MPP_VI_INFO_PROVIDER_H
