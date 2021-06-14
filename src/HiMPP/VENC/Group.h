#ifndef MPP_VENC_GROUP_H
#define MPP_VENC_GROUP_H

#include <vector>
#include <memory>

#include <hi_comm_venc.h>

#include "HiMPP/ASubsystem/ASubsystemItem.h"
#include "Misc/Configurator/Configurator.h"
#include "HiMPP/VPSS/Binder/BindItem.h"
#include "Channel/Channel.h"

namespace hisilicon::mpp::venc {

class Subsystem;

class Group : public ASubsystemItem<Subsystem, Configurator, Channel>,
    public vpss::GroupBindItem {
  public:
    Group(Subsystem *s, int id);
    ~Group();

    Channel *addChannel(int id);
    const std::vector<Channel *> &channels() const;

  protected:
    bool configureImpl() override final;
};

}

#endif // MPP_VENC_GROUP_H
