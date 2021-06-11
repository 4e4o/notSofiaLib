#ifndef MPP_VPSS_GROUP_H
#define MPP_VPSS_GROUP_H

#include <vector>
#include <memory>

#include <hi_comm_vpss.h>

#include "Misc/IdHolder.h"
#include "Misc/Configurator/Configurator.h"
#include "HiMPP/VENC/Channel/IChannelSource.h"
#include "Binder/BindItem.h"

namespace hisilicon::mpp::vpss {

class Channel;
class Subsystem;
class IGroupSource;

class Group : public Holder<Subsystem *>, public IdHolder,
    public Configurator, public VpssBindReceiver,
    public vpss::VpssBindSource, public venc::IChannelSource  {
  public:
    Group(Subsystem *, int id);
    ~Group();

    void setAttributes(VPSS_GRP_ATTR_S *);
    void setParameters(VPSS_GRP_PARAM_S *);

    void setSource(IGroupSource *);

    Channel *addChannel(int id);
    const std::vector<Channel *> &channels() const;

    Subsystem *subsystem() const;

  protected:
    bool configureImpl() override final;

  private:
    HI_S32 receiverBindDeviceId() override final;
    HI_S32 receiverBindChannelId() override final;
    HI_S32 sourceBindDeviceId() override final;
    HI_S32 sourceBindChannelId() override final;

    // venc::IChannelSource
    SIZE_S imgSize() const override final;
    bool pal() const override final;
    PIXEL_FORMAT_E pixelFormat() const override final;

    std::vector<Channel *> m_channels;
    std::unique_ptr<VPSS_GRP_ATTR_S> m_attrs;
    std::unique_ptr<VPSS_GRP_PARAM_S> m_params;
    IGroupSource *m_source;
};

}

#endif // MPP_VPSS_GROUP_H
