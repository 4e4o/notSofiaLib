#include "Device.h"

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

// это маска только для hi3520d в нашем режиме
static void setMask(VI_DEV ViDev, VI_DEV_ATTR_S *pstDevAttr) {
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
    : mpp::vi::Device(p, id),
      m_attr(DEV_ATTR_BT656_2MUX) {
}

bool Device::configureImpl() {
    setMask(id(), &m_attr);
    setAttr(&m_attr);
    return mpp::vi::Device::configureImpl();
}

}
