#ifndef MPP_VPSS_GROUP_H
#define MPP_VPSS_GROUP_H

#include <vector>
#include <memory>

#include <hi_comm_vpss.h>

#include "HiMPP/ASubsystem/InfoSources/IVideoFormatSource.h"
#include "HiMPP/ASubsystem/ASubsystemItem.h"
#include "HiMPP/ASubsystem/AttributesHolder.h"
#include "Misc/Configurator/Configurator.h"
#include "Binder/BindItem.h"
#include "Channel.h"

namespace hisilicon::mpp {
class IVideoCaptureFormatSource;
}

namespace hisilicon::mpp::vpss {

class Subsystem;
class GroupAttributes;
class GroupParameters;

class Group : public ASubsystemItem<Subsystem, Configurator, Channel>,
    public VpssBindItem, public IVideoFormatSource,
    public AttributesHolder<true, GroupAttributes *, GroupParameters *> {
  public:
    Group(Subsystem *, int id);
    ~Group();

    Channel *addChannel(int id);
    const std::vector<Channel *> &channels() const;

    IVideoCaptureFormatSource *source() const;

  protected:
    bool configureImpl() override final;

  private:
    // BindItem
    void setBindedSource(BindItem *bi) override final;
    // IVideoFormatSource
    SIZE_S imgSize() const override final;
    HI_U32 fps() const override final;
    PIXEL_FORMAT_E pixelFormat() const override final;

    IVideoCaptureFormatSource *m_source;
};

}

#endif // MPP_VPSS_GROUP_H
