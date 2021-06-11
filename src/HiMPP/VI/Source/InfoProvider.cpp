#include "InfoProvider.h"
#include "DeviceInfo.h"
#include "ChannelInfo.h"

// когда определён то на каждую не найденную инфу по каналу будет создан фейковый конфиг для захвата
// что позволит захватывать чёрные фреймы
//#define FAKE_NOT_FOUNDED_CHANNEL_INFO

namespace hisilicon::mpp::vi {

InfoProvider::InfoProvider() {
}

InfoProvider::~InfoProvider() {
}

void InfoProvider::setDeviceInfo(const TViDevicesInfo &d) {
    m_devices = d;
}

static ChannelInfo *fakeInfo(DeviceInfo *dev, int chId) {
    ChannelInfo *info = new ChannelInfo(dev, chId);
    TSize size{.width = 352, .height = 240};

    info->setCapSize(size);
    info->setImgSize(size);

    info->setPal(false);
    info->setScanMode(VI_SCAN_PROGRESSIVE);
    info->setPixelFormat(PIXEL_FORMAT_YUV_SEMIPLANAR_422);
    dev->addChannel(info);

    return info;
}

const ChannelInfo *InfoProvider::findChannelInfo(int devId, int chId) const {
    for (auto &device : m_devices) {
        if (device->id() == devId) {
            for (auto &channel : device->channels()) {
                if (channel->id() == chId)
                    return channel;
            }

#ifdef FAKE_NOT_FOUNDED_CHANNEL_INFO
            return fakeInfo(device, chId);
#else
            return nullptr;
#endif
        }
    }

    return nullptr;
}

}
