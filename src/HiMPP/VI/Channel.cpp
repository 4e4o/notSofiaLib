#include "Channel.h"
#include "Device.h"
#include "HiMPP/VI/Source/IChannelInfo.h"
#include "HiMPP/ASubsystem/InfoSources/FrameFormatSource.h"
#include "HiMPP/Misc/Utils.h"
#include "ChannelAttributes.h"

#include <stdexcept>
#include <iostream>

#include <mpi_vi.h>

namespace hisilicon::mpp::vi {

Channel::Channel(Device *d, const IChannelInfo *info, int id)
    : ASubsystemLeaf(d, id),
      vpss::ViBindItem(d, this),
      m_source(info) {
}

Channel::~Channel() {
    HI_MPI_VI_DisableChn(id());
    std::cout << "~vi::Channel " << this << " , " << id() << std::endl;
}

bool Channel::configureImpl() {
    attributes()->setAttributes();

    if (HI_MPI_VI_EnableChn(id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VI_EnableChn failed");

    return true;
}

const IChannelInfo *Channel::source() const {
    if (m_source == nullptr)
        throw std::runtime_error("m_source is not set");

    return m_source;
}

SIZE_S *Channel::createDestSize() const {
    return new SIZE_S{Utils::toMppSize(m_source->imgSize())};
}

SIZE_S Channel::destSize() const {
    if (m_destSize.get() == nullptr)
        m_destSize.reset(createDestSize());

    return *m_destSize.get();
}

SIZE_S Channel::imgSize() const {
    return Utils::toMppSize(m_source->imgSize());
}

HI_U32 Channel::fps() const {
    return attributes()->get<ChannelAttributes::FrameRate>(m_source->fps());
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
    return m_source->pixelFormat();
}

RECT_S Channel::capRect() const {
    return Utils::toMppRect(m_source->capSize());
}

const Device *Channel::device() const {
    return Holder<Device *>::value();
}

}
