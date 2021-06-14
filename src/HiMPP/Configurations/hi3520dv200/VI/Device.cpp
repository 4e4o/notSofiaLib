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

}
