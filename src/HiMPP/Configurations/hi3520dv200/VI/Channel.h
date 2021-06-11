#ifndef MPP_HI3520DV200_VI_CHANNEL_H
#define MPP_HI3520DV200_VI_CHANNEL_H

#include "HiMPP/VI/Channel.h"

namespace hisilicon::mpp::vi::hi3520dv200 {

class Channel : public mpp::vi::Channel {
  public:
    using mpp::vi::Channel::Channel;

  private:
    SIZE_S createDestSize() const override final;
};

}

#endif // MPP_HI3520DV200_VI_CHANNEL_H
