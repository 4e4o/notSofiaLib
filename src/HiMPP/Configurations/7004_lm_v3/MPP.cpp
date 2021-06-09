#include "MPP.h"
#include "HiMPP/VENC/Subsystem.h"

namespace hisilicon::mpp::lm7004v3 {

void MPP::addSubsystems() {
    addViSubsystem();
    addVpssSubsystem();
    addVencSubsystem();

    // это зависит от того с какими параметрами загружен h264e.ko
    // см. комментарии внутри setPoolAllocationMode
    venc()->setPoolAllocationMode(venc::Subsystem::PoolAllocationMode::USER_VB_POOL);
}

}
