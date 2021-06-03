#ifndef MPP_VI_DEVICE_H
#define MPP_VI_DEVICE_H

#include <vector>

#include <hi_comm_vi.h>

#include "Hisilicon/MPP/MPPChild.h"
#include "Utils/IdHolder.h"
#include "Utils/AConfigurator.h"

namespace hisilicon {
namespace mpp {

class ViChannel;

class ViDevice : public MPPChild, public IdHolder, public AConfigurator {
public:
    ViDevice(MPP*, int id);
    ~ViDevice();

    void setAttr(VI_DEV_ATTR_S* attr);

    bool configure();

    ViChannel* addChannel(int id);

private:
    typedef std::vector<ViChannel*> TChannels;
    VI_DEV_ATTR_S* m_attr;
    bool m_enabled;
};

}
}

#endif // MPP_VI_DEVICE_H
