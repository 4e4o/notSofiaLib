#ifndef MPP_HI3520DV200_VI_DEVICE_H
#define MPP_HI3520DV200_VI_DEVICE_H

#include "Hisilicon/MPP/VI/Device.h"

namespace hisilicon {
namespace mpp {
namespace vi {
namespace hi3520dv200 {

class Device : public mpp::vi::Device {
public:
    Device(mpp::vi::Subsystem*, int id);

private:
    bool configureImpl();

    VI_DEV_ATTR_S m_attr;
};

}
}
}
}

#endif // _HI3520DV200
