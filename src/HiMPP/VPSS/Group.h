#ifndef MPP_VPSS_GROUP_H
#define MPP_VPSS_GROUP_H

#include <vector>
#include <memory>

#include <hi_comm_vpss.h>

#include "Utils/IdHolder.h"
#include "Utils/Configurator/Configurator.h"
#include "Binder/BindItem.h"

namespace hisilicon {
namespace mpp {
namespace vpss {

class Channel;
class Subsystem;

class Group : public Holder<Subsystem*>, public IdHolder,
        public Configurator, public VpssBindReceiver {
public:
    Group(Subsystem*, int id);
    ~Group();

    void setAttributes(VPSS_GRP_ATTR_S*);
    void setParameters(VPSS_GRP_PARAM_S*);

    Channel* addChannel(int id);
    const std::vector<Channel*>& channels() const;

protected:
    bool configureImpl();
    bool startImpl();

private:
    Subsystem* subsystem() const;
    HI_S32 receiverBindDeviceId();
    HI_S32 receiverBindChannelId();

    std::vector<Channel*> m_channels;
    std::unique_ptr<VPSS_GRP_ATTR_S> m_attrs;
    std::unique_ptr<VPSS_GRP_PARAM_S> m_params;
};

}
}
}

#endif // MPP_VPSS_GROUP_H
