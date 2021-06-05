#ifndef MPP_VI_DEVICE_H
#define MPP_VI_DEVICE_H

#include <vector>

#include <hi_comm_vi.h>

#include "Hisilicon/MPP/MPPChild.h"
#include "Utils/IdHolder.h"
#include "Utils/Configurator/Configurator.h"

namespace hisilicon {
namespace mpp {
namespace vi {

class Channel;

class Device : public MPPChild, public IdHolder, public Configurator {
public:
    Device(MPP*, int id);
    ~Device();

    void setAttr(VI_DEV_ATTR_S* attr);

    Channel* addChannel(int id, int infoDevId, int infoChId);

protected:
    bool configureImpl();
    bool startImpl();

private:
    Channel* addChannel(int id);

    VI_DEV_ATTR_S* m_attr;
};

}
}
}

#endif // MPP_VI_DEVICE_H
