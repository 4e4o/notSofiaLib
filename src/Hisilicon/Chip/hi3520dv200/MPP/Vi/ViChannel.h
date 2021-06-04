#ifndef MPP_HI3520DV200_VI_CHANNEL_H
#define MPP_HI3520DV200_VI_CHANNEL_H

#include "Hisilicon/MPP/VI/ViChannel.h"

namespace hisilicon {
namespace mpp {
namespace hi3520dv200 {

class ViChannel : public mpp::ViChannel {
public:
    ViChannel(mpp::MPP*, mpp::ViDevice*, int id);
    ~ViChannel();

private:
    TSize createDestSize() const;
    VI_CHN_BIND_ATTR_S createBindAttrs() const;
};

}
}
}

#endif // MPP_HI3520DV200_VI_CHANNEL_H
