#include "ViInfoProvider.h"
#include "HiMPP/VI/Source/ChannelInfo.h"
#include "Boards/nvp6134/Board.h"
#include "ADC/nvp6134/Chip.h"
#include "ADC/nvp6134/ViChannel.h"

#include <stdexcept>

//#define ADD_ONE_CHANNEL_ID 2

namespace boards::nvp6134::mpp::vi {

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

bool InfoProvider::configureImpl() {
    for (auto &chip : value()->nvp()) {
        for (const auto &channel : chip->viChannels()) {
#ifdef ADD_ONE_CHANNEL_ID
            if (channel->id() != ADD_ONE_CHANNEL_ID)
                continue;
#endif
            if (channel->formatDetected()) {
                ChannelInfo *info = new ChannelInfo();

                info->setCapSize(channel->captureSize());
                info->setImgSize(channel->imageSize());
                info->setFps(channel->pal() ? 25 : 30);
                info->setScanMode(toMppScanMode(channel.get()));
                info->setPixelFormat(toMppPixelFormat(channel.get()));
                addChannel({chip->id(), channel->id()}, info);

#ifdef ADD_ONE_CHANNEL_ID
                return true;
#endif
            }
        }
    }

    return true;
}

}
