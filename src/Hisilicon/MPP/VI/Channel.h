#ifndef MPP_VI_CHANNEL_H
#define MPP_VI_CHANNEL_H


#include <memory>

#include <hi_comm_vi.h>

#include "Utils/Configurator/Configurable.h"
#include "Utils/IdHolder.h"
#include "Utils/Size.h"

namespace hisilicon {
namespace mpp {
namespace vi {

class Device;
class ChannelInfo;

class Channel : public IdHolder, public Holder<Device*>,
        public Configurable {
public:
    Channel(Device*, ChannelInfo*, int id);
    ~Channel();

    const Device* device() const;
    SIZE_S destSize() const;
    SIZE_S imgSize() const;
    bool pal() const;
    PIXEL_FORMAT_E pixelFormat() const;

protected:
    void setAttr(VI_CHN_ATTR_S* attr);
    ChannelInfo* info() const;

private:
    bool configureImpl();
    bool startImpl();
    virtual TSize createDestSize() const;

    ChannelInfo* m_info;
    std::unique_ptr<VI_CHN_ATTR_S> m_attr;
};

}
}
}

#endif // MPP_VI_CHANNEL_H
