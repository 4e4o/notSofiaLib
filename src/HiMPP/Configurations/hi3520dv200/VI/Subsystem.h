#ifndef MPP_HI3520DV200_VI_SUBSYSTEM_H
#define MPP_HI3520DV200_VI_SUBSYSTEM_H

#include "HiMPP/VI/Subsystem.h"

namespace hisilicon {
namespace mpp {
namespace vi {
namespace hi3520dv200 {

class Subsystem : public mpp::vi::Subsystem {
public:
    Subsystem(mpp::MPP*);

private:
    void createDevices();
};

}
}
}
}

#endif // MPP_HI3520DV200_VI_SUBSYSTEM_H