#ifndef MPP_VI_DEVICE_H
#define MPP_VI_DEVICE_H

#include <vector>
#include <utility>

#include <hi_comm_vi.h>

#include "Misc/Configurator/Configurator.h"
#include "HiMPP/ASubsystem/ASubsystemItem.h"
#include "Channel.h"

namespace hisilicon::mpp::vi {

class Subsystem;
class IChannelInfo;

class Device : public ASubsystemItem<Subsystem, Configurator, Channel> {
  public:
    Device(Subsystem *, int id);
    ~Device();

    Channel *addChannel(int id);
    const std::vector<Channel *> &channels() const;

  protected:
    typedef std::pair<int, int> TIntPair;

    bool configureImpl() override;
    void setAttributes(VI_DEV_ATTR_S *attr);

  private:
    virtual TIntPair getBindWay(const TIntPair &);
    Channel *addChannel(const IChannelInfo *, int id);
    void bindChannels();

    VI_DEV_ATTR_S *m_attr;
};

}

#endif // MPP_VI_DEVICE_H
