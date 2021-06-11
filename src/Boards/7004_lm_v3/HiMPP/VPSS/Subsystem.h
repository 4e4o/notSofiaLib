#ifndef LM_7003_V3_MPP_VPSS_SUBSYSTEM_H
#define LM_7003_V3_MPP_VPSS_SUBSYSTEM_H

#include "HiMPP/VPSS/Subsystem.h"

namespace boards::lm7004v3::vpss {

class Subsystem : public hisilicon::mpp::vpss::Subsystem {
  public:
    using hisilicon::mpp::vpss::Subsystem::Subsystem;

  private:
    bool configureImpl() override final;
};

}

#endif // LM_7003_V3_MPP_VPSS_SUBSYSTEM_H
