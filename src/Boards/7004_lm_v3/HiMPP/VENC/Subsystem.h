#ifndef LM_7003_V3_MPP_VENC_SUBSYSTEM_H
#define LM_7003_V3_MPP_VENC_SUBSYSTEM_H

#include "HiMPP/VENC/Subsystem.h"

namespace boards::lm7004v3::venc {

class Subsystem : public hisilicon::mpp::venc::Subsystem {
public:
    using hisilicon::mpp::venc::Subsystem::Subsystem;

private:
    bool configureImpl() override final;
};

}

#endif // LM_7003_V3_MPP_VENC_SUBSYSTEM_H
