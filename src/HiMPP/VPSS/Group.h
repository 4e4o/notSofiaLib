#ifndef MPP_VPSS_GROUP_H
#define MPP_VPSS_GROUP_H

#include <vector>
#include <memory>

#include <hi_comm_vpss.h>

#include "Utils/IdHolder.h"
#include "Utils/Configurator/Configurator.h"
#include "Binder/BindItem.h"

namespace hisilicon::mpp::vi {
class Channel;
}

namespace hisilicon::mpp::vpss {

class Channel;
class Subsystem;

class Group : public Holder<Subsystem*>, public IdHolder,
        public Configurator, public VpssBindReceiver {
public:
    Group(Subsystem*, int id);
    ~Group();

    void setAttributes(VPSS_GRP_ATTR_S*);
    void setParameters(VPSS_GRP_PARAM_S*);

    // установить аттрибуты из vi канала
    void setAttributes(vi::Channel*);

    Channel* addChannel(int id);
    const std::vector<Channel*>& channels() const;

protected:
    bool configureImpl() override final;
    bool startImpl() override final;

private:
    HI_S32 receiverBindDeviceId() override final;
    HI_S32 receiverBindChannelId() override final;
    Subsystem* subsystem() const;

    std::vector<Channel*> m_channels;
    std::unique_ptr<VPSS_GRP_ATTR_S> m_attrs;
    std::unique_ptr<VPSS_GRP_PARAM_S> m_params;
};

}

#endif // MPP_VPSS_GROUP_H
