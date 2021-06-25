#ifndef MPP_VPSS_BIND_ITEM_H
#define MPP_VPSS_BIND_ITEM_H

#include <vector>
#include <hi_common.h>

#include "Misc/IdHolder.h"

namespace hisilicon::mpp::vpss {

class Binder;

class BindItem {
  public:
    virtual ~BindItem() { }

    template<class Derived = BindItem>
    Derived * bindedSource() const {
        return dynamic_cast<Derived *>(m_bindedSource);
    }

    template<class Derived = BindItem, class Child>
    static Derived * firstBindedSource(Child *child,
                                       const BindItem *parent = nullptr) {
        const BindItem *provider = parent;

        if constexpr (std::is_base_of<BindItem, Child>::value) {
            if (child->bindedSource() != nullptr)
                provider = child;
        }

        return dynamic_cast<Derived *>(provider->firstBindedSource());
    }

  protected:
    BindItem(IdHolder *dev, IdHolder *chn = nullptr)
        : m_devIdHolder(dev), m_chnIdHolder(chn),
          m_bindedSource(nullptr) {}

    virtual MOD_ID_E bindMode(bool source) = 0;

    virtual HI_S32 bindDeviceId(bool) {
        if (m_devIdHolder == nullptr)
            return 0;

        return m_devIdHolder->id();
    }

    virtual HI_S32 bindChannelId(bool) {
        if (m_chnIdHolder == nullptr)
            return 0;

        return m_chnIdHolder->id();
    }

    virtual void setBindedSource(BindItem *bi) {
        m_bindedSource = bi;
    }

    virtual void addBindedReceiver(BindItem *bi) {
        m_bindedReceivers.push_back(bi);
    }

  private:
    friend class Binder;

    const BindItem *firstBindedSource() const {
        if (m_bindedSource != nullptr)
            return m_bindedSource->firstBindedSource();

        return this;
    }

    IdHolder *m_devIdHolder;
    IdHolder *m_chnIdHolder;
    BindItem *m_bindedSource;
    std::vector<BindItem *> m_bindedReceivers;
};

template<MOD_ID_E T>
class BaseBindItem : public BindItem {
  protected:
    using BindItem::BindItem;

  private:
    MOD_ID_E bindMode(bool) override final {
        return T;
    }
};

using ViBindItem = BaseBindItem<HI_ID_VIU>;
using VpssBindItem = BaseBindItem<HI_ID_VPSS>;
using GroupBindItem = BaseBindItem<HI_ID_GROUP>;
using VdaBindItem = BaseBindItem<HI_ID_VDA>;

}

#endif // MPP_VPSS_BIND_ITEM_H
