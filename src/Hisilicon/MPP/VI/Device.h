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
    const std::vector<Channel*>& channels() const;

protected:
    bool configureImpl();
    bool startImpl();
    void setAttr(VI_DEV_ATTR_S* attr);

private:
    virtual int getBindWay(int i, Channel*);
    Channel* addChannel(ChannelInfo*, int id);
    void bindChannels();
    Subsystem* subsystem() const;

    std::vector<Channel*> m_channels;
    VI_DEV_ATTR_S* m_attr;
};

}
}
}

#endif // MPP_VI_DEVICE_H
