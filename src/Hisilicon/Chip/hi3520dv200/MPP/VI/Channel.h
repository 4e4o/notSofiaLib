#ifndef MPP_HI3520DV200_VI_CHANNEL_H
#define MPP_HI3520DV200_VI_CHANNEL_H

#include "Hisilicon/MPP/VI/Channel.h"

namespace hisilicon {
namespace mpp {
namespace vi {
namespace hi3520dv200 {

class Channel : public mpp::vi::Channel {
public:
    Channel(mpp::MPP*, mpp::vi::Device*, int id);
    ~Channel();

private:
    TSize createDestSize() const;
    VI_CHN_BIND_ATTR_S createBindAttrs() const;
};

}
}
}
}

#endif // MPP_HI3520DV200_VI_CHANNEL_H