#include "ViInfoProvider.h"
#include "Hisilicon/MPP/Source/ViDeviceInfo.h"
#include "Hisilicon/MPP/Source/ViChannelInfo.h"
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

hisilicon::mpp::ViInfoProvider::TViDevicesInfo ViInfoNvp6134Provider::createViInfo(BoardWithNvp6134* b) {
    hisilicon::mpp::ViInfoProvider::TViDevicesInfo result;
    const int chipsCount = b->nvpCount();

    for (int i = 0 ; i < chipsCount ; i++) {
        ViDeviceInfo *dev = new ViDeviceInfo(i);
        ::nvp6134::Chip* chip = b->nvp(i);
        auto& ch = chip->viChannels();

        for (int j = 0 ; j < (int) ch.size() ; j++) {
            ViChannelInfo* channel = new ViChannelInfo(dev, j);

            if (ch[j]->formatDetected()) {
                channel->setCapSize(ch[j]->captureSize());
                channel->setImgSize(ch[j]->imageSize());

/*                switch(ch[j]->pixelFormat()) {
                case ViChannel::OutPixelFormat::YUV_422:
                    channel->setPixelFormat(PIXEL_FORMAT_YUV_SEMIPLANAR_422);
                    break;
                default:
                    throw std::runtime_error("Invalid pixel format from nvp");
                }*/
            }

            // TODO remove it, uncomment upper code
            channel->setPixelFormat(PIXEL_FORMAT_YUV_SEMIPLANAR_422);

            dev->addChannel(channel);
        }

        result.push_back(dev);
    }

    return result;
}

}
}
