#ifndef MPP_SYS_H
#define MPP_SYS_H

#include <hi_type.h>

#include "HiMPP/MPPChild.h"
#include "Misc/Configurator/Configurable.h"

namespace hisilicon::mpp {

class Sys : public MPPChild, public Configurable {
public:
    Sys(MPP*);
    ~Sys();

    void setSysWidthAlign(HI_U32);
    HI_U32 sysWidthAlign() const;

private:
    bool startImpl() override final;

    HI_U32 m_sysWidthAlign;
};

}

#endif // MPP_SYS_H
