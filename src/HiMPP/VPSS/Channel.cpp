#include "Channel.h"
#include "Group.h"
#include "HiMPP/Misc/Utils.h"

#include <stdexcept>
#include <iostream>

#include <mpi_vpss.h>

namespace hisilicon::mpp::vpss {

static VPSS_CHN_ATTR_S *createStandardAttr() {
    return new VPSS_CHN_ATTR_S{};
}

Channel::Channel(Group *g, int id)
    : ASubsystemLeaf(g, id),
      m_attr(createStandardAttr()) {
}

Channel::~Channel() {
    HI_MPI_VPSS_DisableChn(group()->id(), id());
    std::cout << "~vpss::Channel " << this << " , " << id() << std::endl;
}

bool Channel::configureImpl() {
    if (m_attr.get() == nullptr)
        throw std::runtime_error("[vpps::Channel] attr is not set");

    if (HI_MPI_VPSS_SetChnAttr(group()->id(), id(), m_attr.get()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VPSS_SetChnAttr failed");

    if (HI_MPI_VPSS_EnableChn(group()->id(), id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VPSS_EnableChn failed");

    return true;
}

void Channel::setAttributes(VPSS_CHN_ATTR_S *attr) {
    m_attr.reset(attr);
}

const Group *Channel::group() const {
    return Holder<Group *>::value();
}

}
