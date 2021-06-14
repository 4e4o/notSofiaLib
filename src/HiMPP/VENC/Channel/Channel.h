#ifndef MPP_VENC_CHANNEL_H
#define MPP_VENC_CHANNEL_H

#include <memory>

#include <hi_comm_venc.h>

#include "Misc/Configurator/Configurable.h"
#include "Misc/IdHolder.h"
#include "Misc/Size.h"
#include "HiMPP/VB/VBufferizable.h"

namespace hisilicon::mpp::venc {

class Group;
class IAttributesBuilder;
class IGroupSource;
class StreamReader;
class IStreamOut;

class Channel : public IdHolder, public Holder<Group *>,
    public Configurable, public IVBufferizable {
  public:
    Channel(Group *, int id);
    ~Channel();

    enum class BitrateType {
        CBR,
        VBR,
        FIXQP
    };

    void setAttributesBuilder(IAttributesBuilder *);
    void setStreamOut(IStreamOut *, bool getOwnership = true);

    const Group *group() const;
    bool needUserPool() const;
    bool h264Mode() const;
    SIZE_S imgSize() const;

  private:
    bool configureImpl() override final;
    SIZE_S vbImageSize() override final;
    PIXEL_FORMAT_E vbPixelFormat() override final;
    void setAttributes(VENC_CHN_ATTR_S *attr);
    void releaseStreamOut();

    std::unique_ptr<VENC_CHN_ATTR_S> m_attr;
    std::unique_ptr<IAttributesBuilder> m_attrBuilder;
    std::unique_ptr<StreamReader> m_streamReader;
    std::unique_ptr<IStreamOut> m_streamOut;
    bool m_ownsStreamOut;
    IGroupSource *m_source;
};

}

#endif // MPP_VENC_CHANNEL_H
