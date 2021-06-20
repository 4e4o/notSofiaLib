#include "Group.h"
#include "Subsystem.h"

#include <stdexcept>
#include <iostream>

#include <mpi_venc.h>

namespace hisilicon::mpp::venc {

Group::Group(Subsystem *s, int id)
    : ASubsystemItem(s, id),
      vpss::GroupBindItem(this) {
}

Group::~Group() {
    // Сначала удаляем каналы
    ASubsystemItem::clear();

    // потом удаляем группу
    HI_MPI_VENC_DestroyGroup(id());
    std::cout << "~venc::Group " << this << " , " << id() << std::endl;
}

Channel *Group::addChannel(int id) {
    return addSubItem(this, id);
}

const std::vector<Channel *> &Group::channels() const {
    return subItems();
}

bool Group::configureImpl() {
    // создаём группу
    if (HI_MPI_VENC_CreateGroup(id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_CreateGroup failed");

    // стартуем каналы
    return Configurator::configureImpl();
}

}
