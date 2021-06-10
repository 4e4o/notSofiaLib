#include "Channel.h"
#include "HiMPP/VENC/Group.h"
#include "HiMPP/VENC/Subsystem.h"
#include "IAttributesBuilder.h"
#include "HiMPP/Utils.h"
#include "H264AttributesBuilder.h"
#include "IChannelSource.h"
#include "HiMPP/VB/VBPool.h"
#include "StreamLoop.h"
#include "IStreamOut.h"
#include "StreamReader.h"

#include <stdexcept>
#include <iostream>

#include <hi_comm_venc.h>
#include <mpi_venc.h>

namespace hisilicon::mpp::venc {
Channel::Channel(Group* g, int id)
    : IdHolder(id), Holder<Group*>(g),
      m_source(nullptr) {
}

Channel::~Channel() {
    HI_MPI_VENC_StopRecvPic(id());
    HI_MPI_VENC_UnRegisterChn(id());

    if (needUserPool())
        HI_MPI_VENC_DetachVbPool(id());

    HI_MPI_VENC_DestroyChn(id());

    std::cout << "~venc::Channel " << this << " , " << id() << std::endl;
}

void Channel::setStreamOut(IStreamOut* out) {
    m_streamReader.reset(new StreamReader(this));
    m_streamOut.reset(out);

    m_streamReader->setEvent([out](const HI_U8* data, const HI_U32& size) {
        out->write(data, size);
    });
}

void Channel::setSource(IChannelSource* source) {
    m_source = source;
}

void Channel::setAttributesBuilder(IAttributesBuilder *b) {
    m_attrBuilder.reset(b);
}

bool Channel::h264Mode() const {
    if (m_attrBuilder.get() == nullptr)
        throw std::runtime_error("[venc::Channel] m_attrBuilder is not set");

    return dynamic_cast<H264AttributesBuilder*>(m_attrBuilder.get()) != nullptr;
}

bool Channel::needUserPool() const {
    return (group()->subsystem()->poolAllocationMode() == Subsystem::PoolAllocationMode::USER_VB_POOL)
            && h264Mode();
}

bool Channel::configureImpl() {
    if (m_source == nullptr)
        throw std::runtime_error("m_source is not set");

    if (m_streamReader.get() == nullptr)
        throw std::runtime_error("m_streamReader is not set");

    if (m_attrBuilder.get() == nullptr)
        throw std::runtime_error("[venc::Channel] m_attrBuilder is not set");

    m_attr.reset(m_attrBuilder->build(m_source));

    if (m_attr.get() == nullptr)
        throw std::runtime_error("[venc::Channel] attr is not set");

    return true;
}

bool Channel::startImpl() {
    if (HI_MPI_VENC_CreateChn(id(), m_attr.get()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_CreateChn failed");

    if (needUserPool()) {
        // HiMPP Media Processing Software Development Reference.pdf
        // page 512
        if(HI_MPI_VENC_AttachVbPool(id(), group()->subsystem()->pool()->id()) != HI_SUCCESS)
            throw std::runtime_error("HI_MPI_VENC_AttachVbPool failed");
    }

    if (HI_MPI_VENC_RegisterChn(group()->id(), id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_RegisterChn failed");

    if (HI_MPI_VENC_StartRecvPic(id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_StartRecvPic failed");

    StreamLoop* sl = group()->subsystem()->getLoopForChannel();
    m_streamReader->attach(sl);

    return true;
}

void Channel::setAttributes(VENC_CHN_ATTR_S* attr) {
    m_attr.reset(attr);
}

const Group* Channel::group() const {
    return Holder<Group*>::value();
}

SIZE_S Channel::vbImageSize() {
    return m_source->imgSize();
}

PIXEL_FORMAT_E Channel::vbPixelFormat() {
    return m_source->pixelFormat();
}

}
