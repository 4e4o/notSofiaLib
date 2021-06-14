#ifndef MPP_VPSS_GROUP_H
#define MPP_VPSS_GROUP_H

#include <vector>
#include <memory>

#include <hi_comm_vpss.h>

#include "HiMPP/ASubsystem/ASubsystemItem.h"
#include "Misc/Configurator/Configurator.h"
#include "HiMPP/VENC/Channel/IChannelSource.h"
#include "Binder/BindItem.h"
#include "Channel.h"

namespace hisilicon::mpp::vpss {

class Subsystem;
class IGroupSource;

class Group : public ASubsystemItem<Subsystem, Configurator, Channel>,
    public VpssBindItem, public venc::IChannelSource  {
  public:
    Group(Subsystem *, int id);
    ~Group();

    void setAttributes(VPSS_GRP_ATTR_S *);
    void setParameters(VPSS_GRP_PARAM_S *);

    void setSource(IGroupSource *);

    Channel *addChannel(int id);
    const std::vector<Channel *> &channels() const;

  protected:
    bool configureImpl() override final;

  private:
    // venc::IChannelSource
    SIZE_S imgSize() const override final;
    bool pal() const override final;
    PIXEL_FORMAT_E pixelFormat() const override final;

    std::unique_ptr<VPSS_GRP_ATTR_S> m_attrs;
    std::unique_ptr<VPSS_GRP_PARAM_S> m_params;
    IGroupSource *m_source;
};

}

#endif // MPP_VPSS_GROUP_H
