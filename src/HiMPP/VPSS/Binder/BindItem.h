#ifndef MPP_VPSS_BIND_ITEM_H
#define MPP_VPSS_BIND_ITEM_H

#include <hi_common.h>

#include "Misc/IdHolder.h"

namespace hisilicon::mpp::vpss {

class Binder;

class BindItem {
  public:
    virtual ~BindItem() { }

    template<class Derived = BindItem>
    Derived * bindedItem() const {
        return dynamic_cast<Derived *>(m_bindedItem);
    }

  protected:
    BindItem(IdHolder *dev, IdHolder *chn = nullptr)
        : m_devIdHolder(dev), m_chnIdHolder(chn),
          m_bindedItem(nullptr) {}

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

    virtual void setBindedItem(BindItem *bi, bool) {
        m_bindedItem = bi;
    }

  private:
    friend class Binder;

    IdHolder *m_devIdHolder;
    IdHolder *m_chnIdHolder;
    BindItem *m_bindedItem;
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
