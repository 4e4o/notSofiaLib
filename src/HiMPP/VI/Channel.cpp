#include "Channel.h"
#include "Device.h"
#include "HiMPP/VI/Source/ChannelInfo.h"
#include "HiMPP/Utils.h"

#include <stdexcept>
#include <iostream>

#include <hi_comm_vi.h>
#include <mpi_vi.h>

namespace hisilicon {
namespace mpp {
namespace vi {

static VI_CHN_ATTR_S* createStandardAttr() {
    VI_CHN_ATTR_S* attr = new VI_CHN_ATTR_S{};

    attr->bMirror = HI_FALSE;
    attr->bFlip = HI_FALSE;
    attr->enCapSel = VI_CAPSEL_BOTH;
    attr->bChromaResample = HI_FALSE;
    attr->s32SrcFrameRate = -1;
    attr->s32FrameRate = -1;

    return attr;
}

Channel::Channel(Device* d, ChannelInfo* info, int id)
    : IdHolder(id), Holder<Device*>(d),
      m_info(info),
      m_attr(createStandardAttr()) {
}

Channel::~Channel() {
    HI_MPI_VI_DisableChn(id());
    std::cout << "~vi::Channel " << this << " , " << id() << std::endl;
}

SIZE_S Channel::createDestSize() const {
    return Utils::toMppSize(m_info->imgSize());
}

HI_S32 Channel::sourceBindDeviceId() {
    return device()->id();
}

HI_S32 Channel::sourceBindChannelId() {
    return id();
}

bool Channel::configureImpl() {
    if (m_info == NULL)
        throw std::runtime_error("[vi::Channel] Vi info is not set");

    if (m_attr.get() == NULL)
        throw std::runtime_error("[vi::Channel] Vi attr is not set");

    m_attr->enPixFormat = m_info->pixelFormat();
    m_attr->stCapRect = Utils::toMppRect(m_info->capSize());
    m_attr->stDestSize = createDestSize();

    return true;
}

bool Channel::startImpl() {
    if (HI_MPI_VI_SetChnScanMode(id(), m_info->scanMode()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VI_SetChnScanMode failed");

    if (HI_MPI_VI_SetChnAttr(id(), m_attr.get()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VI_SetChnAttr failed");

    if (HI_MPI_VI_EnableChn(id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VI_EnableChn failed");

    return true;
}

void Channel::setAttributes(VI_CHN_ATTR_S* attr) {
    m_attr.reset(attr);
}

SIZE_S Channel::destSize() const {
    return m_attr->stDestSize;
}

RECT_S Channel::capRect() const {
    return m_attr->stCapRect;
}

SIZE_S Channel::imgSize() const {
    return Utils::toMppSize(m_info->imgSize());
}

bool Channel::pal() const {
    return m_info->pal();
}

PIXEL_FORMAT_E Channel::pixelFormat() const {
    return m_attr->enPixFormat;
}

const Device* Channel::device() const {
    return Holder<Device*>::value();
}

}
}
}
