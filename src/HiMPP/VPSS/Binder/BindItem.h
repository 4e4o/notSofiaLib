#ifndef MPP_VPSS_BIND_ITEM_H
#define MPP_VPSS_BIND_ITEM_H

#include <hi_common.h>

namespace hisilicon {
namespace mpp {
namespace vpss {

class BindSource {
public:
    virtual ~BindSource() { }

    virtual MOD_ID_E sourceBindMode() = 0;
    virtual HI_S32 sourceBindDeviceId() = 0;
    virtual HI_S32 sourceBindChannelId() = 0;

protected:
    BindSource() { }
};

class BindReceiver {
public:
    virtual ~BindReceiver() { }

    virtual MOD_ID_E receiverBindMode() = 0;
    virtual HI_S32 receiverBindDeviceId() = 0;
    virtual HI_S32 receiverBindChannelId() = 0;

protected:
    BindReceiver() { }
};

class ViBindSource : public BindSource {
private:
    MOD_ID_E sourceBindMode() {
        return HI_ID_VIU;
    }
};

class VpssBindReceiver : public BindReceiver {
private:
    MOD_ID_E receiverBindMode() {
        return HI_ID_VPSS;
    }
};

}
}
}

#endif // MPP_VPSS_BIND_ITEM_H
