#ifndef MPP_7004_LM_V3_H
#define MPP_7004_LM_V3_H

#include "HiMPP/MPP.h"

namespace hisilicon::mpp::lm7004v3 {

class MPP : public mpp::MPP {
public:
    using mpp::MPP::MPP;

private:
    void addSubsystems() override final;
};

}

#endif // MPP_7004_LM_V3_H
