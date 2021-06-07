#ifndef MPP_VENC_CHANNEL_H
#define MPP_VENC_CHANNEL_H

#include <memory>

#include <hi_comm_venc.h>

#include "Utils/Configurator/Configurable.h"
#include "Utils/IdHolder.h"
#include "Utils/Size.h"

namespace hisilicon::mpp::venc {

class Group;

class Channel : public IdHolder, public Holder<Group*>,
        public Configurable {
public:
    Channel(Group*, int id);
    ~Channel();

    void setAttributes(VENC_CHN_ATTR_S* attr);

    const Group* group() const;

private:
    bool configureImpl() override final;
    bool startImpl() override final;

    std::unique_ptr<VENC_CHN_ATTR_S> m_attr;
};

}

#endif // MPP_VENC_CHANNEL_H
