#include "Device.h"
#include "Channel.h"
#include "Hisilicon/MPP/MPP.h"
#include "Hisilicon/MPP/ElementsFactory.h"
#include "Hisilicon/MPP/VI/Subsystem.h"
#include "Hisilicon/MPP/VI/Source/InfoProvider.h"

#include <stdexcept>
#include <iostream>

#include <mpi_vi.h>

namespace hisilicon {
namespace mpp {
namespace vi {

Device::Device(Subsystem* p, int id)
    : Holder<Subsystem*>(p), IdHolder(id),
      m_attr(NULL) {
}

Device::~Device() {
    // delete channels first
    Configurator::clear();
    HI_MPI_VI_DisableDev(id());
    std::cout << "~vi::Device " << this << " , " << id() << std::endl;
}

Subsystem* Device::subsystem() const {
    return Holder<Subsystem*>::value();
}

Channel* Device::addChannel(ChannelInfo* i, int id) {
    Channel* ch = subsystem()->parent()->factory()->viChannel(this, i, id);
    addItem(ch);
    m_channels.push_back(ch);
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

    bindChannels();
    return Configurator::startImpl();
}

const std::vector<Channel*>& Device::channels() const {
    return m_channels;
}

Channel* Device::addChannel(int id, int infoDevId, int infoChId) {
    InfoProvider* inf = subsystem()->infoProvider();
    ChannelInfo* i = inf->findChannelInfo(infoDevId, infoChId);

    if (i == NULL)
        return NULL;

    return addChannel(i, id);
}

void Device::bindChannels() {
    VI_CHN_BIND_ATTR_S attr;
    auto& chnls = channels();

    for (int i = 0 ; i < (int) chnls.size() ; i++) {
        Channel* ch = chnls[i];

        // HiMPP Media Processing Software Development Reference.pdf
        // page 109

        attr.ViDev = id();
        attr.ViWay = getBindWay(i, ch);

        if (HI_MPI_VI_ChnUnBind(ch->id()) != HI_SUCCESS)
            throw std::runtime_error("HI_MPI_VI_ChnUnBind failed");

        if (HI_MPI_VI_ChnBind(ch->id(), &attr) != HI_SUCCESS)
            throw std::runtime_error("HI_MPI_VI_ChnBind failed");
    }
}

int Device::getBindWay(int i, Channel*) {
    return i;
}

void Device::setAttr(VI_DEV_ATTR_S* attr) {
    m_attr = attr;
}

}
}
}
