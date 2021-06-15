#ifndef MPP_A_SUBSYSTEM_LEAF_H
#define MPP_A_SUBSYSTEM_LEAF_H

#include "Misc/Configurator/Configurable.h"
#include "Misc/IdHolder.h"

namespace hisilicon::mpp {

// Абстрактный лист подсистемы mpp
// (который наследуется от Configurable, но не от Configurator)

template<class Parent>
class ASubsystemLeaf : public IdHolder, public Holder<Parent *>,
    public Configurable {
  public:
    ASubsystemLeaf(Parent *p, int id)
        : IdHolder(id), Holder<Parent*>(p) {}
};

}

#endif // MPP_A_SUBSYSTEM_LEAF_H
