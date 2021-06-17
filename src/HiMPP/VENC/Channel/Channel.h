#ifndef MPP_VENC_CHANNEL_H
#define MPP_VENC_CHANNEL_H

#include <memory>

#include <hi_comm_venc.h>

#include "HiMPP/ASubsystem/InfoSources/IFrameFormatSource.h"
#include "HiMPP/ASubsystem/ASubsystemLeaf.h"
#include "Misc/Size.h"

namespace hisilicon::mpp {
class IVideoFormatSource;
}

namespace hisilicon::mpp::venc {

class Group;
class ChannelAttributes;
class StreamReader;
class IStreamOut;

class Channel : public ASubsystemLeaf<Group>, public IFrameFormatSource {
  public:
    Channel(Group *, int id);
    ~Channel();

    void setAttributes(ChannelAttributes *);
    ChannelAttributes *attributes() const;

    void setStreamOut(IStreamOut *, bool getOwnership = true);

    const Group *group() const;
    bool needUserPool() const;
    bool h264Mode() const;
    // IFrameFormatSource
    SIZE_S imgSize() const override final;

  private:
    bool configureImpl() override final;
    // IFrameFormatSource
    PIXEL_FORMAT_E pixelFormat() const override final;

    void releaseStreamOut();

    std::unique_ptr<ChannelAttributes> m_attrBuilder;
    std::unique_ptr<StreamReader> m_streamReader;
    std::unique_ptr<IStreamOut> m_streamOut;
    bool m_ownsStreamOut;
    IVideoFormatSource *m_source;
};

}

#endif // MPP_VENC_CHANNEL_H
