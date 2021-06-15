#ifndef MPP_VPSS_CHANNEL_H
#define MPP_VPSS_CHANNEL_H

#include <memory>

#include <hi_comm_vpss.h>

#include "HiMPP/ASubsystem/ASubsystemLeaf.h"
#include "Misc/Size.h"

namespace hisilicon::mpp::vpss {

class Group;

class Channel : public ASubsystemLeaf<Group> {
  public:
    Channel(Group *, int id);
    ~Channel();

    void setAttributes(VPSS_CHN_ATTR_S *attr);

    const Group *group() const;

  private:
    bool configureImpl() override final;

    std::unique_ptr<VPSS_CHN_ATTR_S> m_attr;
};

}

#endif // MPP_VPSS_CHANNEL_H
