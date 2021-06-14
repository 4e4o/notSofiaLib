#include "Channel.h"
#include "Device.h"
#include "HiMPP/VI/Source/ChannelInfo.h"
#include "HiMPP/Misc/Utils.h"

#include <stdexcept>
#include <iostream>

#include <mpi_vi.h>

namespace hisilicon::mpp::vi {

static VI_CHN_ATTR_S *createStandardAttr() {
    VI_CHN_ATTR_S *attr = new VI_CHN_ATTR_S{};

    attr->enCapSel = VI_CAPSEL_BOTH;
    attr->s32SrcFrameRate = -1;
    attr->s32FrameRate = -1;

    return attr;
}

Channel::Channel(Device *d, const ChannelInfo *info, int id)
    : IdHolder(id), Holder<Device*>(d),
      ViBindItem(d, this),
      m_info(info),
      m_attr(createStandardAttr()) {
}

Channel::~Channel() {
    HI_MPI_VI_DisableChn(id());
    std::cout << "~vi::Channel " << this << " , " << id() << std::endl;
}

bool Channel::configureImpl() {
    if (m_info == nullptr)
        throw std::runtime_error("[vi::Channel] Vi info is not set");

    if (m_attr.get() == nullptr)
        throw std::runtime_error("[vi::Channel] Vi attr is not set");

    m_attr->enPixFormat = pixelFormat();
    m_attr->stCapRect = capRect();
    m_attr->stDestSize = destSize();

    if (HI_MPI_VI_SetChnScanMode(id(), m_info->scanMode()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VI_SetChnScanMode failed");

    if (HI_MPI_VI_SetChnAttr(id(), m_attr.get()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VI_SetChnAttr failed");

    if (HI_MPI_VI_EnableChn(id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VI_EnableChn failed");

    return true;
}

SIZE_S *Channel::createDestSize() const {
    return new SIZE_S{Utils::toMppSize(m_info->imgSize())};
}

SIZE_S Channel::destSize() const {
    if (m_destSize.get() == nullptr)
        m_destSize.reset(createDestSize());

    return *m_destSize.get();
}

RECT_S Channel::capRect() const {
    return Utils::toMppRect(m_info->capSize());
}

SIZE_S Channel::imgSize() const {
    return Utils::toMppSize(m_info->imgSize());
}

HI_U32 Channel::fps() const {
    return m_info->fps();
}

PIXEL_FORMAT_E Channel::pixelFormat() const {
    return m_info->pixelFormat();
}

const Device *Channel::device() const {
    return Holder<Device *>::value();
}

SIZE_S Channel::vbImageSize() {
    // Тут нет уверенности что именно imgSize() юзать надо,
    // проверено destSize, capSize -
    // так же работает буффер без ошибок
    // значит будем юзать наименьшее значение
    return imgSize();
}

PIXEL_FORMAT_E Channel::vbPixelFormat() {
    return pixelFormat();
}

}
