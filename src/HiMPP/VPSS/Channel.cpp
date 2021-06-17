#include "Channel.h"
#include "Group.h"
#include "HiMPP/Misc/Utils.h"
#include "ChannelAttributes.h"

#include <stdexcept>
#include <iostream>

#include <mpi_vpss.h>

namespace hisilicon::mpp::vpss {

Channel::Channel(Group *g, int id)
    : ASubsystemLeaf(g, id),
      m_attrBuilder(new ChannelAttributes()) {
}

Channel::~Channel() {
    HI_MPI_VPSS_DisableChn(group()->id(), id());
    std::cout << "~vpss::Channel " << this << " , " << id() << std::endl;
}

bool Channel::configureImpl() {
    if (m_attrBuilder.get() == nullptr)
        throw std::runtime_error("[vpps::Channel] m_attrBuilder is not set");

    std::unique_ptr<VPSS_CHN_ATTR_S> attrs(m_attrBuilder->build());

    if (attrs.get() == nullptr)
        throw std::runtime_error("[vpps::Channel] attrs is nullptr");

    if (HI_MPI_VPSS_SetChnAttr(group()->id(), id(), attrs.get()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VPSS_SetChnAttr failed");

    if (HI_MPI_VPSS_EnableChn(group()->id(), id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VPSS_EnableChn failed");

    return true;
}

void Channel::setAttributes(ChannelAttributes *attr) {
    m_attrBuilder.reset(attr);
}

ChannelAttributes *Channel::attributes() const {
    return m_attrBuilder.get();
}

const Group *Channel::group() const {
    return Holder<Group *>::value();
}

}
