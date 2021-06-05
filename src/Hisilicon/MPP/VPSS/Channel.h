#ifndef MPP_VPSS_CHANNEL_H
#define MPP_VPSS_CHANNEL_H

#include <memory>

#include <hi_comm_vpss.h>

#include "Utils/Configurator/Configurable.h"
#include "Utils/IdHolder.h"
#include "Utils/Size.h"

namespace hisilicon {
namespace mpp {
namespace vpss {

class Group;

class Channel : public IdHolder, public Holder<Group*>,
        public Configurable {
public:
    Channel(Group*, int id);
    ~Channel();

    void setAttributes(VPSS_CHN_ATTR_S* attr);

    const Group* group() const;

private:
    bool configureImpl();
    bool startImpl();

    std::unique_ptr<VPSS_CHN_ATTR_S> m_attr;
};

}
}
}

#endif // MPP_VPSS_CHANNEL_H
