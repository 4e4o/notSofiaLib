#ifndef MPP_VPSS_CHANNEL_H
#define MPP_VPSS_CHANNEL_H

#include <memory>

#include <hi_comm_vpss.h>

#include "HiMPP/ASubsystem/AttributesHolder.h"
#include "HiMPP/ASubsystem/ASubsystemLeaf.h"
#include "Misc/Size.h"

namespace hisilicon::mpp::vpss {

class Group;
class ChannelAttributes;

class Channel : public ASubsystemLeaf<Group>,
    public AttributesHolder<true, ChannelAttributes *> {
  public:
    using ASubsystemLeaf::ASubsystemLeaf;
    ~Channel();

    const Group *group() const;

  private:
    bool configureImpl() override final;
};

}

#endif // MPP_VPSS_CHANNEL_H
