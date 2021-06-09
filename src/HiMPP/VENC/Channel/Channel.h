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
class IChannelSource;

class Channel : public IdHolder, public Holder<Group*>,
        public Configurable, public IBufferizable {
public:
    Channel(Group*, int id);
    ~Channel();

    enum class Codec {
        H264
    };

    enum class BitrateType {
        CBR,
        VBR,
        FIXQP
    };

    void setAttributesBuilder(IAttributesBuilder*);
    void setSource(IChannelSource* source);

    const Group* group() const;

    bool needUserPool() const;

private:
    bool configureImpl() override final;
    bool startImpl() override final;
    SIZE_S bufferImageSize() override final;
    PIXEL_FORMAT_E bufferPixelFormat() override final;
    bool h264Mode() const;
    void setAttributes(VENC_CHN_ATTR_S* attr);

    std::unique_ptr<VENC_CHN_ATTR_S> m_attr;
    std::unique_ptr<IAttributesBuilder> m_attrBuilder;
    IChannelSource* m_source;
};

}

#endif // MPP_VENC_CHANNEL_H
