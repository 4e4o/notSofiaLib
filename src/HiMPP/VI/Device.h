#ifndef MPP_VI_DEVICE_H
#define MPP_VI_DEVICE_H

#include <vector>

#include <hi_comm_vi.h>

#include "Misc/Configurator/Configurator.h"
#include "HiMPP/ASubsystem/ASubsystemItem.h"
#include "Channel.h"

namespace hisilicon::mpp::vi {

class Subsystem;
class ChannelInfo;

class Device : public ASubsystemItem<Subsystem, Configurator, Channel> {
  public:
    Device(Subsystem *, int id);
    ~Device();

    Channel *addChannel(int id, int infoDevId, int infoChId);
    const std::vector<Channel *> &channels() const;

  protected:
    bool configureImpl() override;
    void setAttributes(VI_DEV_ATTR_S *attr);

  private:
    virtual int getBindWay(int i, Channel *);
    Channel *addChannel(const ChannelInfo *, int id);
    void bindChannels();

    VI_DEV_ATTR_S *m_attr;
};

}

#endif // MPP_VI_DEVICE_H
