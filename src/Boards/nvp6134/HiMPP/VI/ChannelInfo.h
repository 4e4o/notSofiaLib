#ifndef BOARD_NVP6134_VI_CHANNEL_INFO_H
#define BOARD_NVP6134_VI_CHANNEL_INFO_H

#include "HiMPP/VI/Source/ChannelInfo.h"
#include "Misc/Holder.h"

namespace nvp6134 {
class ViChannel;
}

namespace boards::nvp6134::mpp::vi {

class ChannelInfo : public ::hisilicon::mpp::vi::ChannelInfo,
    public Holder<::nvp6134::ViChannel *> {
  public:
    ChannelInfo(::nvp6134::ViChannel *);

    ::nvp6134::ViChannel *viChannel() const;
};

}

#endif // BOARD_NVP6134_VI_CHANNEL_INFO_H
