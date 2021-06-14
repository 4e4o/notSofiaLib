#ifndef MPP_A_GROUP_OF_ITEMS_H
#define MPP_A_GROUP_OF_ITEMS_H

#include <vector>
#include <concepts>

#include "Misc/Configurator/Configurator.h"
#include "Misc/Configurator/ConfigurableFactory.h"

namespace hisilicon::mpp {

// Абстрактная группа элементов,
// конфигурируемая и создаваемая через фактори

template<class TConfiguratorBase, class TSubItem>
requires std::derived_from<TConfiguratorBase, Configurator>
class AGroupOfItems : public TConfiguratorBase {
  protected:
    AGroupOfItems(ConfigurableFactory *f)
        : m_factory(f) {}

    template<typename... ConstrArgs>
    TSubItem *addSubItem(ConstrArgs... args) {
        TSubItem *subItem = m_factory->template create<TSubItem>(args...);
        TConfiguratorBase::addItemBack(subItem);
        m_subItems.push_back(subItem);
        return subItem;
    }

    const std::vector<TSubItem *> &subItems() const {
        return m_subItems;
    }

    ConfigurableFactory *factory() const {
        return m_factory;
    }

  private:
    std::vector<TSubItem *> m_subItems;
    ConfigurableFactory *m_factory;
};

}

#endif // MPP_A_GROUP_OF_ITEMS_H
