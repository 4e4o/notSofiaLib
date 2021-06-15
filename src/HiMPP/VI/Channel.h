#ifndef MPP_VI_CHANNEL_H
#define MPP_VI_CHANNEL_H


#include <memory>

#include <hi_comm_vi.h>

#include "Misc/Configurator/Configurable.h"
#include "Misc/IdHolder.h"
#include "Misc/Size.h"
#include "HiMPP/VPSS/Binder/BindItem.h"
#include "HiMPP/ASubsystem/InfoSources/IVideoCaptureFormatSource.h"

namespace hisilicon::mpp::vi {

class Device;
class ChannelInfo;

class Channel : public IdHolder, public Holder<Device *>,
    public Configurable, public vpss::ViBindItem,
    public IVideoCaptureFormatSource {
  public:
    Channel(Device *, const ChannelInfo *, int id);
    ~Channel();

    const Device *device() const;

    // IVideoCaptureFormatSource
    SIZE_S destSize() const override final;
    SIZE_S imgSize() const override final;
    HI_U32 fps() const override final;
    PIXEL_FORMAT_E pixelFormat() const override final;

  protected:
    RECT_S capRect() const;

  private:
    virtual SIZE_S *createDestSize() const;
    bool configureImpl() override final;

    const ChannelInfo *m_info;
    std::unique_ptr<VI_CHN_ATTR_S> m_attr;
    mutable std::unique_ptr<SIZE_S> m_destSize;
};

}

#endif // MPP_VI_CHANNEL_H
