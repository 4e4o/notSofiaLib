#ifndef MPP_VI_DEVICE_H
#define MPP_VI_DEVICE_H

#include <vector>

#include <hi_comm_vi.h>

#include "Utils/IdHolder.h"
#include "Utils/Configurator/Configurator.h"

namespace hisilicon {
namespace mpp {
namespace vi {

class Channel;
class Subsystem;
class ChannelInfo;

class Device : public Holder<Subsystem*>, public IdHolder, public Configurator {
public:
    Device(Subsystem*, int id);
    ~Device();

    Channel* addChannel(int id, int infoDevId, int infoChId);

protected:
    bool configureImpl();
    bool startImpl();

    void setAttr(VI_DEV_ATTR_S* attr);

private:
    Channel* addChannel(ChannelInfo*, int id);
    void bindChannels();
    virtual int getBindWay(int i, Channel*);

    Subsystem* subsystem() const;

    VI_DEV_ATTR_S* m_attr;
};

}
}
}

#endif // MPP_VI_DEVICE_H
