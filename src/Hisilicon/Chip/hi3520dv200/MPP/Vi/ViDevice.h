#ifndef MPP_HI3520DV200_VI_DEVICE_H
#define MPP_HI3520DV200_VI_DEVICE_H

#include "Hisilicon/MPP/Vi/ViDevice.h"

namespace hisilicon {
namespace mpp {
namespace hi3520dv200 {

class ViDevice : public mpp::ViDevice {
public:
    ViDevice(mpp::MPP *, int id);

    bool configure();
private:
    VI_DEV_ATTR_S m_attr;
};

}
}
}

#endif // _HI3520DV200
