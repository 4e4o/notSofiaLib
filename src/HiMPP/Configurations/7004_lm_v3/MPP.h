#ifndef MPP_7004_LM_V3_H
#define MPP_7004_LM_V3_H

#include "HiMPP/Configurations/hi3520dv200/MPP.h"

namespace hisilicon::mpp::lm7004v3 {

class MPP : public mpp::hi3520dv200::MPP {
public:
    using mpp::hi3520dv200::MPP::MPP;

private:
    void addSubsystems() override final;
};

}

#endif // MPP_7004_LM_V3_H
