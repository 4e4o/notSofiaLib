#include "ViInfoProvider.h"
#include "HiMPP/VI/Source/DeviceInfo.h"
#include "HiMPP/VI/Source/ChannelInfo.h"
#include "Boards/nvp6134/Board.h"
#include "ADC/nvp6134/Chip.h"
#include "ADC/nvp6134/ViChannel.h"

#include <stdexcept>

namespace boards::nvp6134::mpp::vi {

using hisilicon::mpp::vi::DeviceInfo;
using hisilicon::mpp::vi::ChannelInfo;
using ::nvp6134::ViChannel;

static PIXEL_FORMAT_E toMppPixelFormat(ViChannel *ch) {
    switch (ch->pixelFormat()) {
    case ViChannel::OutPixelFormat::YUV_422:
        return PIXEL_FORMAT_YUV_SEMIPLANAR_422;
        break;
    default:
        throw std::runtime_error("Invalid pixel format from nvp");
    }
}

static VI_SCAN_MODE_E toMppScanMode(ViChannel *ch) {
    // TODO more formats
    switch (ch->videoFormat()) {
    case ViChannel::DF_1080P_NTSC:
        return VI_SCAN_PROGRESSIVE;
    default:
        return VI_SCAN_INTERLACED;
    }
}

InfoProvider::InfoProvider(Board *b)
    : Holder<Board *>(b) {
    initInfo();
}

void InfoProvider::initInfo() {
    hisilicon::mpp::vi::InfoProvider::TViDevicesInfo result;
    auto &nvp = value()->nvp();

    for (auto &chip : nvp) {
        DeviceInfo *dev = new DeviceInfo(chip->id());
        auto &ch = chip->viChannels();

        for (int j = 0 ; j < (int) ch.size() ; j++) {
            ChannelInfo *info = new ChannelInfo(dev, j);
            auto &channel = ch[j];

            if (ch[j]->formatDetected()) {
                info->setCapSize(channel->captureSize());
                info->setImgSize(channel->imageSize());
                info->setFps(channel->pal() ? 25 : 30);
                info->setScanMode(toMppScanMode(channel.get()));
                info->setPixelFormat(toMppPixelFormat(channel.get()));
                dev->addChannel(info);
            }
        }

        result.push_back(dev);
    }

    setDeviceInfo(result);
}

}
