#ifndef MPP_HI3520DV200_VI_CHANNEL_H
#define MPP_HI3520DV200_VI_CHANNEL_H

#include "HiMPP/VI/Channel.h"

namespace hisilicon {
namespace mpp {
namespace vi {
namespace hi3520dv200 {

class Channel : public mpp::vi::Channel {
public:
    Channel(mpp::vi::Device*, ChannelInfo*, int id);

private:
    SIZE_S createDestSize() const;
};

}
}
}
}

#endif // MPP_HI3520DV200_VI_CHANNEL_H
