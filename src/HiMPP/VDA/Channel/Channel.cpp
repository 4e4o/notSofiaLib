#include "Channel.h"
#include "HiMPP/Misc/Utils.h"
#include "Attributes.h"
#include "VdaDataReader.h"
#include "HiMPP/VDA/Subsystem.h"
#include "HiMPP/ASubsystem/ReadLoop/ReadLoop.h"
#include "HiMPP/ASubsystem/InfoSources/IVideoFormatSource.h"

#include <stdexcept>
#include <iostream>

#include <mpi_vda.h>

namespace hisilicon::mpp::vda {

Channel::Channel(ChannelAttributes *attr, VdaDataReader *reader, Subsystem *s,
                 int id)
    : ASubsystemLeaf(s, id),
      vpss::VdaBindItem(nullptr, this),
      m_attrBuilder(attr),
      m_vdaReader(reader),
      m_source(nullptr) {
}

Channel::~Channel() {
    HI_MPI_VDA_StopRecvPic(id());
    HI_MPI_VDA_DestroyChn(id());

    std::cout << "~vda::Channel " << this << " , " << id() << std::endl;
}

bool Channel::configureImpl() {
    if (m_source == nullptr)
        throw std::runtime_error("m_source is not set");

    if (m_vdaReader.get() == nullptr)
        throw std::runtime_error("m_vdaReader is not set");

    if (m_attrBuilder.get() == nullptr)
        throw std::runtime_error("[vda::Channel] m_attrBuilder is not set");

    std::unique_ptr<VDA_CHN_ATTR_S> attrs(m_attrBuilder->build(m_source));

    if (attrs.get() == nullptr)
        throw std::runtime_error("[vda::Channel] attr is nullptr");

    HI_S32 aaa = HI_MPI_VDA_CreateChn(id(), attrs.get());

    if (aaa != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VDA_CreateChn failed");

    if (HI_MPI_VDA_StartRecvPic(id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VDA_StartRecvPic failed");

    ReadLoop *l = subsystem()->parent()->assignLoop();
    m_vdaReader->attach(l);

    return true;
}

void Channel::setBindedItem(BindItem *bi, bool source) {
    BindItem::setBindedItem(bi, source);

    if (source) {
        m_source = bindedItem<IVideoFormatSource>();

        if (m_source == nullptr)
            throw std::runtime_error("BindItem must realize IVideoFormatSource");
    }
}

const Subsystem *Channel::subsystem() const {
    return Holder<Subsystem *>::value();
}

}
