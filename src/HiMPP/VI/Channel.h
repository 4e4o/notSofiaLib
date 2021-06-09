#ifndef MPP_VI_CHANNEL_H
#define MPP_VI_CHANNEL_H


#include <memory>

#include <hi_comm_vi.h>

#include "Utils/Configurator/Configurable.h"
#include "Utils/IdHolder.h"
#include "Utils/Size.h"
#include "HiMPP/VPSS/Binder/BindItem.h"
#include "HiMPP/VPSS/IGroupSource.h"
#include "HiMPP/VB/VBufferizable.h"

namespace hisilicon::mpp::vi {

class Device;
class ChannelInfo;

class Channel : public IdHolder, public Holder<Device*>,
        public Configurable, public vpss::ViBindSource,
        public vpss::IGroupSource, public IBufferizable {
public:
    Channel(Device*, ChannelInfo*, int id);
    ~Channel();

    void setAttributes(VI_CHN_ATTR_S* attr);

    const Device* device() const;

    SIZE_S destSize() const override final;
    SIZE_S imgSize() const override final;
    bool pal() const override final;
    PIXEL_FORMAT_E pixelFormat() const override final;

protected:
    RECT_S capRect() const;

private:
    virtual SIZE_S createDestSize() const;
    bool configureImpl() override final;
    bool startImpl() override final;
    HI_S32 sourceBindDeviceId() override final;
    HI_S32 sourceBindChannelId() override final;
    SIZE_S bufferImageSize() override final;
    PIXEL_FORMAT_E bufferPixelFormat() override final;


    ChannelInfo* m_info;
    std::unique_ptr<VI_CHN_ATTR_S> m_attr;
};

}

#endif // MPP_VI_CHANNEL_H
