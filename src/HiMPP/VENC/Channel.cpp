#include "Channel.h"
#include "Group.h"
#include "HiMPP/Utils.h"

#include <stdexcept>
#include <iostream>

#include <hi_comm_venc.h>
#include <mpi_venc.h>

namespace hisilicon::mpp::venc {

static VENC_CHN_ATTR_S* createStandardAttr() {
    VENC_CHN_ATTR_S* attr = new VENC_CHN_ATTR_S{};


    return attr;
}

Channel::Channel(Group* g, int id)
    : IdHolder(id), Holder<Group*>(g),
      m_attr(createStandardAttr()) {
}

Channel::~Channel() {
    HI_MPI_VENC_StopRecvPic(id());
    HI_MPI_VENC_UnRegisterChn(id());
    HI_MPI_VENC_DestroyChn(id());

    // TODO destroy vd pool

    std::cout << "~venc::Channel " << this << " , " << id() << std::endl;
}

bool Channel::configureImpl() {
    if (m_attr.get() == nullptr)
        throw std::runtime_error("[venc::Channel] attr is not set");

    return true;
}

bool Channel::startImpl() {
    if (HI_MPI_VENC_CreateChn(id(), m_attr.get()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_CreateChn failed");

    // TODO create vb pool
    // HiMPP Media Processing Software Development Reference.pdf
    // page 512

    if (HI_MPI_VENC_RegisterChn(group()->id(), id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_RegisterChn failed");

    if (HI_MPI_VENC_StartRecvPic(id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_StartRecvPic failed");

    return true;
}

void Channel::setAttributes(VENC_CHN_ATTR_S* attr) {
    m_attr.reset(attr);
}

const Group* Channel::group() const {
    return Holder<Group*>::value();
}

}
