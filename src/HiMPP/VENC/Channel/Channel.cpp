#include "Channel.h"
#include "HiMPP/VENC/Group.h"
#include "HiMPP/ASubsystem/InfoSources/IVideoFormatSource.h"
#include "HiMPP/VENC/Subsystem.h"
#include "H264Attributes.h"
#include "HiMPP/Misc/Utils.h"
#include "HiMPP/VB/VBPool.h"
#include "StreamReader.h"

#include <stdexcept>
#include <iostream>

#include <mpi_venc.h>

namespace hisilicon::mpp::venc {

Channel::Channel(Group *g, int id)
    : ASubsystemLeaf(g, id),
      m_source(g->bindedSource<IVideoFormatSource>()),
      m_streamReader(new StreamReader(this)) {
}

Channel::~Channel() {
    HI_MPI_VENC_StopRecvPic(id());
    HI_MPI_VENC_UnRegisterChn(id());

    if (needUserPool())
        HI_MPI_VENC_DetachVbPool(id());

    HI_MPI_VENC_DestroyChn(id());

    std::cout << "~venc::Channel " << this << " , " << id() << std::endl;
}

StreamReader *Channel::streamReader() const {
    return m_streamReader.get();
}

bool Channel::h264Mode() const {
    if (attributes() == nullptr)
        throw std::runtime_error("[venc::Channel] attributes is not set");

    return dynamic_cast<H264Attributes *>(attributes()) != nullptr;
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

IVideoFormatSource *Channel::source() const {
    if (m_source == nullptr)
        throw std::runtime_error("m_source is not set");

    return m_source;
}

bool Channel::configureImpl() {
    if (m_streamReader.get() == nullptr)
        throw std::runtime_error("m_streamReader is not set");

    if (attributes() == nullptr)
        throw std::runtime_error("[venc::Channel] attributes is not set");

    VENC_CHN_ATTR_S *attrs = attributes()->createAttributes();

    if (HI_MPI_VENC_CreateChn(id(), attrs) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_CreateChn failed");

    attributes()->onChannelCreated();

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
