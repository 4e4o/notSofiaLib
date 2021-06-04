#include "Device.h"
#include "Channel.h"
#include "Hisilicon/MPP/MPP.h"
#include "Hisilicon/MPP/ElementsFactory.h"

#include <stdexcept>

#include <mpi_vi.h>

namespace hisilicon {
namespace mpp {
namespace vi {

Device::Device(MPP *p, int id)
    : MPPChild(p), IdHolder(id),
      m_attr(NULL),
      m_enabled(false) {
}

Device::~Device() {
    // TODO order? channels/dev
    AConfigurator::clear();

    if (m_enabled)
        HI_MPI_VI_DisableDev(id());
}

Channel* Device::addChannel(int id) {
    Channel *ch = parent()->factory()->viChannel(parent(), this, id);
    addItem(ch);
    return ch;
}

bool Device::configure() {
    if (m_enabled)
        throw std::runtime_error("vi::Device already configured");

    if (m_attr == NULL)
        throw std::runtime_error("vi::Device attributes not set");

    if (HI_MPI_VI_SetDevAttr(id(), m_attr) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VI_SetDevAttr failed");

    if (HI_MPI_VI_EnableDev(id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VI_EnableDev failed");

    m_enabled = true;

    return AConfigurator::configure();
}

void Device::setAttr(VI_DEV_ATTR_S* attr) {
    m_attr = attr;
}

}
}
}
