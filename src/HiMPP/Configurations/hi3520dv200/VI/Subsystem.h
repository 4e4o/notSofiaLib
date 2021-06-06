#ifndef MPP_HI3520DV200_VI_SUBSYSTEM_H
#define MPP_HI3520DV200_VI_SUBSYSTEM_H

#include "HiMPP/VI/Subsystem.h"

namespace hisilicon::mpp::vi::hi3520dv200 {

class Subsystem : public mpp::vi::Subsystem {
public:
    using mpp::vi::Subsystem::Subsystem;

private:
    void createDevices() override;
};

}

#endif // MPP_HI3520DV200_VI_SUBSYSTEM_H
