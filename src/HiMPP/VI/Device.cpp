#include "Device.h"
#include "Channel.h"
#include "HiMPP/MPP.h"
#include "HiMPP/VI/Subsystem.h"
#include "HiMPP/VI/Source/InfoProvider.h"

#include <stdexcept>
#include <iostream>

#include <mpi_vi.h>

namespace hisilicon::mpp::vi {

Device::Device(Subsystem *p, int id)
    : ASubsystemItem(p, id),
      m_attr(nullptr) {
}

Device::~Device() {
    // delete channels first
    ASubsystemItem::clear();
    HI_MPI_VI_DisableDev(id());
    std::cout << "~vi::Device " << this << " , " << id() << std::endl;
}

Channel *Device::addChannel(const ChannelInfo *i, int id) {
    return addSubItem(this, i, id);
}

const std::vector<Channel *> &Device::channels() const {
    return subItems();
}

bool Device::configureImpl() {
    if (m_attr == nullptr)
        throw std::runtime_error("vi::Device attributes not set");

    if (HI_MPI_VI_SetDevAttr(id(), m_attr) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VI_SetDevAttr failed");

    if (HI_MPI_VI_EnableDev(id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VI_EnableDev failed");

    bindChannels();
    return Configurator::configureImpl();
}

Channel *Device::addChannel(int id, int infoDevId, int infoChId) {
    InfoProvider *inf = subsystem()->infoProvider();
    const ChannelInfo *i = inf->findChannelInfo(infoDevId, infoChId);

    if (i == nullptr)
        return nullptr;

    return addChannel(i, id);
}

void Device::bindChannels() {
    VI_CHN_BIND_ATTR_S attr;
    auto &chnls = channels();

    for (int i = 0 ; i < (int) chnls.size() ; i++) {
        Channel *ch = chnls[i];

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

int Device::getBindWay(int i, Channel *) {
    return i;
}

void Device::setAttributes(VI_DEV_ATTR_S *attr) {
    m_attr = attr;
}

}
