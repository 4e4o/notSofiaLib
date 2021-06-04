#include "ViInfoProvider.h"
#include "Hisilicon/MPP/VI/Source/ViDeviceInfo.h"
#include "Hisilicon/MPP/VI/Source/ViChannelInfo.h"
#include "Board.h"
#include "ADC/nvp6134/Chip.h"
#include "ADC/nvp6134/ViChannel.h"

#include <stdexcept>

namespace boards {
namespace nvp6134 {

using hisilicon::mpp::ViDeviceInfo;
using hisilicon::mpp::ViChannelInfo;
using ::nvp6134::ViChannel;

ViInfoNvp6134Provider::ViInfoNvp6134Provider(BoardWithNvp6134 *b) :
    ViInfoProvider(createViInfo(b)) {
}

static PIXEL_FORMAT_E toMppPixelFormat(ViChannel* ch) {
    switch(ch->pixelFormat()) {
    case ViChannel::OutPixelFormat::YUV_422:
        return PIXEL_FORMAT_YUV_SEMIPLANAR_422;
        break;
    default:
        throw std::runtime_error("Invalid pixel format from nvp");
    }
}

static VI_SCAN_MODE_E toMppScanMode(ViChannel* ch) {
    switch(ch->videoFormat()) {
    case ViChannel::DF_1080P_NTSC:
        return VI_SCAN_PROGRESSIVE;
    default:
        return VI_SCAN_INTERLACED;
    }
}

hisilicon::mpp::ViInfoProvider::TViDevicesInfo ViInfoNvp6134Provider::createViInfo(BoardWithNvp6134* b) {
    hisilicon::mpp::ViInfoProvider::TViDevicesInfo result;
    const int chipsCount = b->nvpCount();

    for (int i = 0 ; i < chipsCount ; i++) {
        ViDeviceInfo *dev = new ViDeviceInfo(i);
        ::nvp6134::Chip* chip = b->nvp(i);
        auto& ch = chip->viChannels();

        for (int j = 0 ; j < (int) ch.size() ; j++) {
            ViChannelInfo* info = new ViChannelInfo(dev, j);
            auto& channel = ch[j];

            if (ch[j]->formatDetected()) {
                info->setCapSize(channel->captureSize());
                info->setImgSize(channel->imageSize());
                info->setPal(channel->pal());
                info->setScanMode(toMppScanMode(channel.get()));
                info->setPixelFormat(toMppPixelFormat(channel.get()));
            } else {
                // TODO remove it and don't append channel info
                // фейковый конфиг чтоб пример пока работал

                TSize size;
                size.width = 704 / 2;
                size.height = 240;

                info->setCapSize(size);
                info->setImgSize(size);

                info->setPal(false);
                info->setScanMode(VI_SCAN_PROGRESSIVE);
                info->setPixelFormat(PIXEL_FORMAT_YUV_SEMIPLANAR_422);
            }

            dev->addChannel(info);
        }

        result.push_back(dev);
    }

    return result;
}

}
}
