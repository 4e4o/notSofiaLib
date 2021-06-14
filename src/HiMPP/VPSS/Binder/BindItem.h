#ifndef MPP_VPSS_BIND_ITEM_H
#define MPP_VPSS_BIND_ITEM_H

#include <hi_common.h>

#include "Misc/IdHolder.h"

namespace hisilicon::mpp::vpss {

class BindItem {
  public:
    virtual ~BindItem() { }

    virtual MOD_ID_E bindMode(bool source) = 0;

    virtual HI_S32 bindDeviceId(bool) {
        return m_devIdHolder->id();
    }

    virtual HI_S32 bindChannelId(bool) {
        if (m_chnIdHolder == nullptr)
            return 0;

        return m_chnIdHolder->id();
    }

  protected:
    BindItem(IdHolder *dev, IdHolder *chn = nullptr)
        : m_devIdHolder(dev), m_chnIdHolder(chn) {}

  private:
    IdHolder *m_devIdHolder;
    IdHolder *m_chnIdHolder;
};

class ViBindItem : public BindItem {
  protected:
    using BindItem::BindItem;

  private:
    MOD_ID_E bindMode(bool) override final {
        return HI_ID_VIU;
    }
};

class VpssBindItem : public BindItem {
  protected:
    using BindItem::BindItem;

  private:
    MOD_ID_E bindMode(bool) override final {
        return HI_ID_VPSS;
    }
};

class GroupBindItem : public BindItem {
  protected:
    using BindItem::BindItem;

  private:
    MOD_ID_E bindMode(bool) override final {
        return HI_ID_GROUP;
    }
};

}

#endif // MPP_VPSS_BIND_ITEM_H
