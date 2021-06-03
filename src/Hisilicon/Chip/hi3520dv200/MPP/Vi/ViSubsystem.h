#ifndef MPP_HI3520DV200_VI_SUBSYSTEM_H
#define MPP_HI3520DV200_VI_SUBSYSTEM_H

#include "Hisilicon/MPP/Vi/ViSubsystem.h"

namespace hisilicon {
namespace mpp {
namespace hi3520dv200 {

class ViSubsystem : public mpp::ViSubsystem {
public:
    ViSubsystem(mpp::MPP*);
    ~ViSubsystem();

    bool configure();
};

}
}
}

#endif // MPP_HI3520DV200_VI_SUBSYSTEM_H
