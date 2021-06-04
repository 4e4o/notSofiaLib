#ifndef MPP_VI_CHANNEL_H
#define MPP_VI_CHANNEL_H

#include <hi_comm_vi.h>

#include "Hisilicon/MPP/MPPChild.h"
#include "Utils/IConfigurable.h"
#include "Utils/IdHolder.h"
#include "Utils/Size.h"

namespace hisilicon {
namespace mpp {

class ViDevice;
class ViChannelInfo;

class ViChannel : public MPPChild, public IdHolder,
        public Holder<ViDevice*>, public IConfigurable {
public:
    ViChannel(MPP*, ViDevice*, int id);
    ~ViChannel();

    ViDevice* device() const;

    void setAttr(VI_CHN_ATTR_S* attr);

    // должен быть вызван после setAttr
    void setInfo(ViChannelInfo* info);

    bool configure();

    SIZE_S destSize() const;
    SIZE_S imgSize() const;
    bool pal() const;

protected:
    ViChannelInfo* info() const;
    void bind(const VI_CHN_BIND_ATTR_S&);

private:
    virtual TSize createDestSize() const;
    virtual VI_CHN_BIND_ATTR_S createBindAttrs() const;

    bool m_enabled;
    ViChannelInfo* m_info;
    std::unique_ptr<VI_CHN_ATTR_S> m_attr;
};

}
}

#endif // MPP_VI_CHANNEL_H
