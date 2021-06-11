#include "Group.h"
#include "Channel/Channel.h"
#include "HiMPP/MPP.h"
#include "Subsystem.h"
#include "HiMPP/VI/Channel.h"

#include <stdexcept>
#include <iostream>

#include <mpi_venc.h>

namespace hisilicon::mpp::venc {

Group::Group(Subsystem* p, int id)
    : Holder<Subsystem*>(p), IdHolder(id) {
}

Group::~Group() {
    // Сначала каналы удаляем
    Configurator::clear();

    // потом удаляем группу
    HI_MPI_VENC_DestroyGroup(id());
    std::cout << "~venc::Group " << this << " , " << id() << std::endl;
}

Subsystem* Group::subsystem() const {
    return Holder<Subsystem*>::value();
}

HI_S32 Group::receiverBindDeviceId() {
    return id();
}

HI_S32 Group::receiverBindChannelId() {
    return 0;
}

Channel* Group::addChannel(int id) {
    Channel* ch = subsystem()->parent()->create<Channel>(this, id);
    addItemBack(ch);
    m_channels.push_back(ch);
    return ch;
}

bool Group::startImpl() {
    // создаём группу
    if (HI_MPI_VENC_CreateGroup(id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_CreateGroup failed");

    // стартуем каналы
    return Configurator::startImpl();
}

const std::vector<Channel*>& Group::channels() const {
    return m_channels;
}

}
