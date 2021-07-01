#ifndef MPP_VENC_CHANNEL_H
#define MPP_VENC_CHANNEL_H

#include <memory>

#include <hi_comm_venc.h>

#include "HiMPP/ASubsystem/InfoSources/IFrameFormatSource.h"
#include "HiMPP/ASubsystem/ASubsystemLeaf.h"
#include "HiMPP/ASubsystem/AttributesHolder.h"
#include "Misc/Size.h"

namespace hisilicon::mpp {
class IVideoFormatSource;
}

namespace hisilicon::mpp::venc {

class Group;
class ChannelAttributes;
class StreamReader;
class IStreamOut;

class Channel : public ASubsystemLeaf<Group>, public IFrameFormatSource,
    public AttributesHolder<false, ChannelAttributes *> {
  public:
    Channel(Group *, int id);
    ~Channel();

    StreamReader *streamReader() const;

    const Group *group() const;
    bool needUserPool() const;
    bool h264Mode() const;
    // IFrameFormatSource
    SIZE_S imgSize() const override final;

    IVideoFormatSource *source() const;

  private:
    bool configureImpl() override final;
    // IFrameFormatSource
    PIXEL_FORMAT_E pixelFormat() const override final;

    IVideoFormatSource *m_source;
    std::unique_ptr<StreamReader> m_streamReader;
};

}

#endif // MPP_VENC_CHANNEL_H
