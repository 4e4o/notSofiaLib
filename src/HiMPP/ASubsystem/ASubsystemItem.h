#ifndef MPP_A_SUBSYSTEM_ITEM_H
#define MPP_A_SUBSYSTEM_ITEM_H

#include "AGroupOfItems.h"
#include "Misc/IdHolder.h"

namespace hisilicon::mpp {

// Абстрактный элемент подсистемы mpp

template<class TSubsystem, class TConfiguratorBase, class TSubItem>
class ASubsystemItem : public Holder<TSubsystem *>, public IdHolder,
    public AGroupOfItems<TConfiguratorBase, TSubItem> {
  public:
    ASubsystemItem(TSubsystem *s, int id)
        : Holder<TSubsystem*>(s), IdHolder(id),
          AGroupOfItems<TConfiguratorBase, TSubItem>(s->parent()) {}

    template<class DerivedSubsystem = TSubsystem>
    requires std::derived_from<DerivedSubsystem, TSubsystem>
    DerivedSubsystem * subsystem() const {
        return static_cast<DerivedSubsystem *>(Holder<TSubsystem *>::value());
    }
};

}

#endif // MPP_A_SUBSYSTEM_ITEM_H
