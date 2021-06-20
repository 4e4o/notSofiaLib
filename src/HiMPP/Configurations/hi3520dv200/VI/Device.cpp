#include "Device.h"
#include "Subsystem.h"

namespace hisilicon::mpp::vi::hi3520dv200 {

static VI_DEV_ATTR_S DEV_ATTR_BT656_2MUX = {
    VI_MODE_BT656,
    VI_WORK_MODE_2Multiplex,
    { 0xFF000000, 0x0 },
    VI_SCAN_PROGRESSIVE,
    { -1, -1, -1, -1 },
    VI_INPUT_DATA_YVYU,
    {},
    VI_PATH_BYPASS,
    VI_DATA_TYPE_YUV,
    HI_TRUE
};

static void set4ch720pMask(VI_DEV ViDev, VI_DEV_ATTR_S *pstDevAttr) {
    switch (ViDev % 2) {
    case 0:
        pstDevAttr->au32CompMask[0] = 0xFF000000;
        break;
    case 1:
        pstDevAttr->au32CompMask[0] = 0xFF0000;
        break;
    }
}

Device::Device(mpp::vi::Subsystem *p, int id)
    : mpp::vi::Device(p, id) {
    initMode();
}

void Device::initMode() {
    using vi::hi3520dv200::Subsystem;
    Subsystem *s = subsystem<Subsystem>();

    switch (s->hiMode()) {
    case Subsystem::HI3520DV200_MODE::MODE_4CH_72OP: {
        m_attr = DEV_ATTR_BT656_2MUX;
        set4ch720pMask(id(), &m_attr);
        setAttributes(&m_attr);
        break;
    }
    default:
        throw std::runtime_error("[vi::hi3520dv200::Device] Unsupported HI3520DV200_MODE");
    }
}

Device::TIntPair Device::getBindWay(const TIntPair &i) {
    using vi::hi3520dv200::Subsystem;
    Subsystem *s = subsystem<Subsystem>();

    switch (s->hiMode()) {
    case Subsystem::HI3520DV200_MODE::MODE_4CH_72OP: {
        // Hi3520D／Hi3515A／Hi3515C H.264 CODEC Processor Data Sheet.pdf
        // page 616
        if (i.first == 0) {
            if (i.second == 0)
                return {0, 0};
            else if ((i.second == 2) || (i.second == 3))
                return {0, 1};
        } else if (i.first == 1) {
            if (i.second == 4)
                return {1, 0};
            else if ((i.second == 6) || (i.second == 7))
                return {1, 1};
        }
        break;
    }
    default:
        break;
    }

    throw std::runtime_error("[vi::hi3520dv200::Device] Unsupported HI3520DV200_MODE");
}


}
