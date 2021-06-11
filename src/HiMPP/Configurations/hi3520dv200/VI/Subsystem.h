#ifndef MPP_HI3520DV200_VI_SUBSYSTEM_H
#define MPP_HI3520DV200_VI_SUBSYSTEM_H

#include "HiMPP/VI/Subsystem.h"

namespace hisilicon::mpp::vi::hi3520dv200 {

class Subsystem : public mpp::vi::Subsystem {
  public:
    Subsystem(::hisilicon::mpp::MPP *);

    enum class HI3520DV200_MODE {
        // Hi3520D／Hi3515A／Hi3515C H.264 CODEC Processor Data Sheet.pdf
        // page 616
        MODE_4CH_72OP,
        MODE_UNKNOWN
    };

    void setHiMode(const HI3520DV200_MODE &);
    const HI3520DV200_MODE &hiMode() const;

  protected:
    bool configureImpl() override;

  private:
    void initMode();

    HI3520DV200_MODE m_mode;
};

}

#endif // MPP_HI3520DV200_VI_SUBSYSTEM_H
