#ifndef MPP_A_SUBSYSTEM_H
#define MPP_A_SUBSYSTEM_H

#include "HiMPP/Misc/MPPChild.h"
#include "AGroupOfItems.h"

namespace hisilicon::mpp {

// Абстрактная подсистема mpp

template<class TConfiguratorBase, class TSubItem>
class ASubsystem : public MPPChild,
    public AGroupOfItems<TConfiguratorBase, TSubItem> {
  public:
    ASubsystem(MPP *p)
        : MPPChild(p),
          AGroupOfItems<TConfiguratorBase, TSubItem>(p) {}
};

}

#endif // MPP_A_SUBSYSTEM_H
