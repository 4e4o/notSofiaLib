#include "InfoProvider.h"
#include "ChannelInfo.h"
#include "Boards/nvp6134/Board.h"
#include "ADC/nvp6134/Chip.h"
#include "ADC/nvp6134/ViChannel.h"

#include <stdexcept>

//#define ADD_ONE_CHANNEL_ID 2

namespace boards::nvp6134::mpp::vi {

bool InfoProvider::configureImpl() {
    for (auto &chip : value()->nvp()) {
        for (auto &channel : chip->viChannels()) {
#ifdef ADD_ONE_CHANNEL_ID
            if (channel->id() != ADD_ONE_CHANNEL_ID)
                continue;
#endif
            if (channel->formatDetected()) {
                ChannelInfo *info = new ChannelInfo(channel.get());
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
