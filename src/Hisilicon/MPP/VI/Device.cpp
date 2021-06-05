#include "Device.h"
#include "Channel.h"
#include "Hisilicon/MPP/MPP.h"
#include "Hisilicon/MPP/ElementsFactory.h"
#include "Hisilicon/MPP/VI/Source/InfoProvider.h"
#include "Hisilicon/MPP/VI/Source/DeviceInfo.h"
#include "Hisilicon/MPP/VI/Source/ChannelInfo.h"

#include <stdexcept>
#include <iostream>

#include <mpi_vi.h>

namespace hisilicon {
namespace mpp {
namespace vi {

Device::Device(MPP *p, int id)
    : MPPChild(p), IdHolder(id),
      m_attr(NULL) {
}

Device::~Device() {
    // delete channels first
    Configurator::clear();
    HI_MPI_VI_DisableDev(id());
    std::cout << "~Device " << this << " , " << id() << std::endl;
}

Channel* Device::addChannel(int id) {
    Channel *ch = parent()->factory()->viChannel(parent(), this, id);
    addItem(ch);
    return ch;
}

Channel* Device::addChannel(int id, int infoDevId, int infoChId) {
    InfoProvider* inf = parent()->viSourceInfo();
    ChannelInfo* i = inf->findChannelInfo(infoDevId, infoChId);

    if (i == NULL)
        return NULL;

    mpp::vi::Channel* ch = addChannel(id);
    ch->setInfo(i);
    return ch;
}

bool Device::configureImpl() {
    if (m_attr == NULL)
        throw std::runtime_error("vi::Device attributes not set");

    return Configurator::configureImpl();
}

bool Device::startImpl() {
    if (HI_MPI_VI_SetDevAttr(id(), m_attr) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VI_SetDevAttr failed");

    if (HI_MPI_VI_EnableDev(id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VI_EnableDev failed");

    return Configurator::startImpl();
}

void Device::setAttr(VI_DEV_ATTR_S* attr) {
    m_attr = attr;
}

}
}
}
