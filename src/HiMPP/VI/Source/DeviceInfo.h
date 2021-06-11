#ifndef MPP_VI_DEVICE_INFO_H
#define MPP_VI_DEVICE_INFO_H

#include <vector>

#include "Misc/IdHolder.h"

namespace hisilicon::mpp::vi {

class ChannelInfo;

class DeviceInfo : public IdHolder {
  public:
    typedef std::vector<ChannelInfo *> TViChannelsInfo;

    using IdHolder::IdHolder;
    ~DeviceInfo();

    void addChannel(ChannelInfo *);
    const TViChannelsInfo &channels() const;

  private:
    TViChannelsInfo m_channels;
};

}

#endif // MPP_VI_DEVICE_INFO_H
