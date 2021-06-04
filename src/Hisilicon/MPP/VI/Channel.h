#ifndef MPP_VI_CHANNEL_H
#define MPP_VI_CHANNEL_H

#include <hi_comm_vi.h>

#include "Hisilicon/MPP/MPPChild.h"
#include "Utils/IConfigurable.h"
#include "Utils/IdHolder.h"
#include "Utils/Size.h"

namespace hisilicon {
namespace mpp {
namespace vi {

class Device;
class ChannelInfo;

class Channel : public MPPChild, public IdHolder,
        public Holder<Device*>, public IConfigurable {
public:
    Channel(MPP*, Device*, int id);
    ~Channel();

    Device* device() const;

    void setAttr(VI_CHN_ATTR_S* attr);

    // должен быть вызван после setAttr
    void setInfo(ChannelInfo* info);

    bool configure();

    SIZE_S destSize() const;
    SIZE_S imgSize() const;
    bool pal() const;

protected:
    ChannelInfo* info() const;
    void bind(const VI_CHN_BIND_ATTR_S&);

private:
    virtual TSize createDestSize() const;
    virtual VI_CHN_BIND_ATTR_S createBindAttrs() const;

    bool m_enabled;
    ChannelInfo* m_info;
    std::unique_ptr<VI_CHN_ATTR_S> m_attr;
};

}
}
}

#endif // MPP_VI_CHANNEL_H
