#include "Channel.h"
#include "HiMPP/VENC/Group.h"
#include "HiMPP/ASubsystem/InfoSources/IVideoFormatSource.h"
#include "HiMPP/VENC/Subsystem.h"
#include "H264Attributes.h"
#include "HiMPP/Misc/Utils.h"
#include "HiMPP/VB/VBPool.h"
#include "IStreamOut.h"
#include "StreamReader.h"

#include <stdexcept>
#include <iostream>

#include <mpi_venc.h>

namespace hisilicon::mpp::venc {

Channel::Channel(Group *g, int id)
    : ASubsystemLeaf(g, id),
      m_ownsStreamOut(true),
      m_source(g->bindedItem<IVideoFormatSource>()) {
}

Channel::~Channel() {
    releaseStreamOut();

    HI_MPI_VENC_StopRecvPic(id());
    HI_MPI_VENC_UnRegisterChn(id());

    if (needUserPool())
        HI_MPI_VENC_DetachVbPool(id());

    HI_MPI_VENC_DestroyChn(id());

    std::cout << "~venc::Channel " << this << " , " << id() << std::endl;
}

void Channel::releaseStreamOut() {
    if (!m_ownsStreamOut)
        m_streamOut.release();
}

void Channel::setStreamOut(IStreamOut *out, bool getOwnership) {
    if (m_streamReader.get() == nullptr)
        m_streamReader.reset(new StreamReader(this));

    releaseStreamOut();
    m_streamOut.reset(out);
    m_ownsStreamOut = getOwnership;

    m_streamReader->setEvent([out](const HI_U8 * data, const HI_U32 & size) {
        out->write(data, size);
    });
}

void Channel::setAttributes(ChannelAttributes *b) {
    m_attrBuilder.reset(b);
}

ChannelAttributes *Channel::attributes() const {
    return m_attrBuilder.get();
}

bool Channel::h264Mode() const {
    if (m_attrBuilder.get() == nullptr)
        throw std::runtime_error("[venc::Channel] m_attrBuilder is not set");

    return dynamic_cast<H264Attributes *>(m_attrBuilder.get()) != nullptr;
}

SIZE_S Channel::imgSize() const {
    return m_source->imgSize();
}

PIXEL_FORMAT_E Channel::pixelFormat() const {
    return m_source->pixelFormat();
}

bool Channel::needUserPool() const {
    return (group()->subsystem()->poolAllocationMode() ==
            Subsystem::PoolAllocationMode::USER_VB_POOL)
           && h264Mode();
}

bool Channel::configureImpl() {
    if (m_source == nullptr)
        throw std::runtime_error("m_source is not set");

    if (m_streamReader.get() == nullptr)
        throw std::runtime_error("m_streamReader is not set");

    if (m_attrBuilder.get() == nullptr)
        throw std::runtime_error("[venc::Channel] m_attrBuilder is not set");

    std::unique_ptr<VENC_CHN_ATTR_S> attrs(m_attrBuilder->build(m_source));

    if (attrs.get() == nullptr)
        throw std::runtime_error("[venc::Channel] attr is nullptr");

    if (HI_MPI_VENC_CreateChn(id(), attrs.get()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_CreateChn failed");

    if (needUserPool()) {
        // HiMPP Media Processing Software Development Reference.pdf
        // page 512
        if (HI_MPI_VENC_AttachVbPool(id(),
                                     group()->subsystem()->pool()->id()) != HI_SUCCESS)
            throw std::runtime_error("HI_MPI_VENC_AttachVbPool failed");
    }

    if (HI_MPI_VENC_RegisterChn(group()->id(), id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_RegisterChn failed");

    if (HI_MPI_VENC_StartRecvPic(id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_StartRecvPic failed");

    ReadLoop *l = group()->subsystem()->parent()->assignLoop();
    m_streamReader->attach(l);

    return true;
}

const Group *Channel::group() const {
    return Holder<Group *>::value();
}

}
