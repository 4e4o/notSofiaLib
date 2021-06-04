#ifndef MPP_VI_DEVICE_H
#define MPP_VI_DEVICE_H

#include <vector>

#include <hi_comm_vi.h>

#include "Hisilicon/MPP/MPPChild.h"
#include "Utils/IdHolder.h"
#include "Utils/AConfigurator.h"

namespace hisilicon {
namespace mpp {
namespace vi {

class Channel;

class Device : public MPPChild, public IdHolder, public AConfigurator {
public:
    Device(MPP*, int id);
    ~Device();

    void setAttr(VI_DEV_ATTR_S* attr);

    bool configure();

    Channel* addChannel(int id);

private:
    typedef std::vector<Channel*> TChannels;
    VI_DEV_ATTR_S* m_attr;
    bool m_enabled;
};

}
}
}

#endif // MPP_VI_DEVICE_H
