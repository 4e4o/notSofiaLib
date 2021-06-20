#include "Channel.h"
#include "Device.h"
#include "HiMPP/VI/Source/ChannelInfo.h"
#include "HiMPP/ASubsystem/InfoSources/FrameFormatSource.h"
#include "HiMPP/Misc/Utils.h"
#include "ChannelAttributes.h"

#include <stdexcept>
#include <iostream>

#include <mpi_vi.h>

namespace hisilicon::mpp::vi {

Channel::Channel(Device *d, const ChannelInfo *info, int id)
    : ASubsystemLeaf(d, id),
      vpss::ViBindItem(d, this),
      m_info(info),
      m_attrBuilder(new ChannelAttributes()) {
}

Channel::~Channel() {
    HI_MPI_VI_DisableChn(id());
    std::cout << "~vi::Channel " << this << " , " << id() << std::endl;
}

bool Channel::configureImpl() {
    if (m_info == nullptr)
        throw std::runtime_error("[vi::Channel] Vi info is not set");

    if (m_attrBuilder.get() == nullptr)
        throw std::runtime_error("[vi::Channel] Vi m_attrBuilder is not set");

    std::unique_ptr<VI_CHN_ATTR_S> attrs(m_attrBuilder->build(this, m_info));

    if (attrs.get() == nullptr)
        throw std::runtime_error("[vi::Channel] Vi attr is nullptr");

    if (HI_MPI_VI_SetChnScanMode(id(), m_info->scanMode()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VI_SetChnScanMode failed");

    if (HI_MPI_VI_SetChnAttr(id(), attrs.get()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VI_SetChnAttr failed");

    if (HI_MPI_VI_EnableChn(id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VI_EnableChn failed");

    return true;
}

void Channel::setAttributes(ChannelAttributes *a) {
    m_attrBuilder.reset(a);
}

ChannelAttributes *Channel::attributes() const {
    return m_attrBuilder.get();
}

SIZE_S *Channel::createDestSize() const {
    return new SIZE_S{Utils::toMppSize(m_info->imgSize())};
}

SIZE_S Channel::destSize() const {
    if (m_destSize.get() == nullptr)
        m_destSize.reset(createDestSize());

    return *m_destSize.get();
}

SIZE_S Channel::imgSize() const {
    return Utils::toMppSize(m_info->imgSize());
}

HI_U32 Channel::fps() const {
    return m_attrBuilder->get<ChannelAttributes::FrameRate>(m_info->fps());
}

const IFrameFormatSource *Channel::vbFormatInfo() const {
    // Для буфера надо возвращать размер захватываемых данных
    // Сначало здесь было imgSize - т.е. результирующий размер картинки
    // и это работало, потому что я запускал на двух камерах пример
    // и буферы второй камеры юзались для первой.
    // Потом я отключил вторую камеру, оставил первую, а у меня в первая камера cvbs
    // для неё imgSize() != destSize() и получал ошибки vb fail.
    // Ну и по логике тут и надо размер захвата юзать.
    return new FrameFormatSource(destSize(), pixelFormat());
}

PIXEL_FORMAT_E Channel::pixelFormat() const {
    return m_info->pixelFormat();
}

const Device *Channel::device() const {
    return Holder<Device *>::value();
}

RECT_S Channel::capRect() const {
    return Utils::toMppRect(m_info->capSize());
}

}
