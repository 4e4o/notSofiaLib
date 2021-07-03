#ifndef MPP_VI_CHANNEL_H
#define MPP_VI_CHANNEL_H

#include <memory>

#include <hi_comm_vi.h>

#include "HiMPP/ASubsystem/InfoSources/IVideoCaptureFormatSource.h"
#include "HiMPP/ASubsystem/ASubsystemLeaf.h"
#include "HiMPP/ASubsystem/AttributesHolder.h"
#include "HiMPP/VPSS/Binder/BindItem.h"
#include "Misc/Size.h"

namespace hisilicon::mpp::vi {

class Device;
class IChannelInfo;
class ChannelAttributes;

class Channel : public ASubsystemLeaf<Device>, public vpss::ViBindItem,
    public IVideoCaptureFormatSource,
    public AttributesHolder<true, ChannelAttributes *> {
  public:
    Channel(Device *, const IChannelInfo *, int id);
    ~Channel();

    const Device *device() const;

    // IVideoCaptureFormatSource
    SIZE_S destSize() const override final;
    SIZE_S imgSize() const override final;
    HI_U32 fps() const override final;
    PIXEL_FORMAT_E pixelFormat() const override final;

    const IFrameFormatSource *vbFormatInfo() const;
    const IChannelInfo *source() const;

    template<class Child>
    static int associatedChannelId(Child *child,
                                   const BindItem *parent = nullptr) {
        const Channel *viChannel = associatedChannel<Child>(child, parent);

        if (viChannel == nullptr)
            return -1;

        return viChannel->id();
    }

    template<class Child>
    static const Channel *associatedChannel(Child *child,
                                            const BindItem *parent = nullptr) {
        return BindItem::firstBindedSource<const Channel>(child, parent);
    }

  protected:
    RECT_S capRect() const;

  private:
    virtual SIZE_S *createDestSize() const;
    bool configureImpl() override final;

    const IChannelInfo *m_source;
    mutable std::unique_ptr<SIZE_S> m_destSize;
};

}

#endif // MPP_VI_CHANNEL_H
