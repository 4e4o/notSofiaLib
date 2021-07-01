#include "Channel.h"
#include "Group.h"
#include "ChannelAttributes.h"

#include <stdexcept>
#include <iostream>

#include <mpi_vpss.h>

namespace hisilicon::mpp::vpss {

Channel::~Channel() {
    HI_MPI_VPSS_DisableChn(group()->id(), id());
    std::cout << "~vpss::Channel " << this << " , " << id() << std::endl;
}

bool Channel::configureImpl() {
    attributes()->setAttributes();

    if (HI_MPI_VPSS_EnableChn(group()->id(), id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VPSS_EnableChn failed");

    return true;
}

const Group *Channel::group() const {
    return Holder<Group *>::value();
}

}
