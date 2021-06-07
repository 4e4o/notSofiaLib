#ifndef MPP_VENC_GROUP_H
#define MPP_VENC_GROUP_H

#include <vector>
#include <memory>

#include <hi_comm_venc.h>

#include "Utils/IdHolder.h"
#include "Utils/Configurator/Configurator.h"
#include "HiMPP/VPSS/Binder/BindItem.h"

namespace hisilicon::mpp::venc {

class Channel;
class Subsystem;

class Group : public Holder<Subsystem*>, public IdHolder,
        public Configurator, public vpss::VpssBindReceiver {
public:
    Group(Subsystem*, int id);
    ~Group();

    Channel* addChannel(int id);
    const std::vector<Channel*>& channels() const;

protected:
    bool startImpl() override final;

private:
    HI_S32 receiverBindDeviceId() override final;
    HI_S32 receiverBindChannelId() override final;
    Subsystem* subsystem() const;

    std::vector<Channel*> m_channels;
};

}

#endif // MPP_VENC_GROUP_H
