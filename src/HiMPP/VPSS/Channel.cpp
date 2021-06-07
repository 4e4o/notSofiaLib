#include "Channel.h"
#include "Group.h"
#include "HiMPP/Utils.h"

#include <stdexcept>
#include <iostream>

#include <hi_comm_vpss.h>
#include <mpi_vpss.h>

namespace hisilicon::mpp::vpss {

static VPSS_CHN_ATTR_S* createStandardAttr() {
    VPSS_CHN_ATTR_S* attr = new VPSS_CHN_ATTR_S{};

    attr->bSpEn = HI_FALSE;
    attr->bFrameEn = HI_TRUE;
    attr->stFrame.u32Color[VPSS_FRAME_WORK_LEFT] = 0xff00;
    attr->stFrame.u32Color[VPSS_FRAME_WORK_RIGHT] = 0xff00;
    attr->stFrame.u32Color[VPSS_FRAME_WORK_BOTTOM] = 0xff00;
    attr->stFrame.u32Color[VPSS_FRAME_WORK_TOP] = 0xff00;
    attr->stFrame.u32Width[VPSS_FRAME_WORK_LEFT] = 2;
    attr->stFrame.u32Width[VPSS_FRAME_WORK_RIGHT] = 2;
    attr->stFrame.u32Width[VPSS_FRAME_WORK_TOP] = 2;
    attr->stFrame.u32Width[VPSS_FRAME_WORK_BOTTOM] = 2;

    return attr;
}

Channel::Channel(Group* g, int id)
    : IdHolder(id), Holder<Group*>(g),
      m_attr(createStandardAttr()) {
}

Channel::~Channel() {
    HI_MPI_VPSS_DisableChn(group()->id(), id());
    std::cout << "~vpss::Channel " << this << " , " << id() << std::endl;
}

bool Channel::configureImpl() {
    if (m_attr.get() == nullptr)
        throw std::runtime_error("[vpps::Channel] attr is not set");

    return true;
}

bool Channel::startImpl() {
    if (HI_MPI_VPSS_SetChnAttr(group()->id(), id(), m_attr.get()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VPSS_SetChnAttr failed");

    if (HI_MPI_VPSS_EnableChn(group()->id(), id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VPSS_EnableChn failed");

    return true;
}

void Channel::setAttributes(VPSS_CHN_ATTR_S* attr) {
    m_attr.reset(attr);
}

const Group* Channel::group() const {
    return Holder<Group*>::value();
}

}
