#include "Group.h"
#include "Channel.h"
#include "HiMPP/MPP.h"
#include "Subsystem.h"
#include "HiMPP/ASubsystem/InfoSources/IVideoCaptureFormatSource.h"
#include "GroupAttributes.h"

#include <stdexcept>
#include <iostream>

#include <mpi_vpss.h>

namespace hisilicon::mpp::vpss {

Group::Group(Subsystem *s, int id)
    : ASubsystemItem(s, id),
      VpssBindItem(this),
      m_source(nullptr) {
}

Group::~Group() {
    // сначала останавливаем группу
    HI_MPI_VPSS_StopGrp(id());

    // потом каналы
    ASubsystemItem::clear();

    // потом удаляем группу
    HI_MPI_VPSS_DestroyGrp(id());
    std::cout << "~vpss::Group " << this << " , " << id() << std::endl;
}

Channel *Group::addChannel(int id) {
    return addSubItem(this, id);
}

const std::vector<Channel *> &Group::channels() const {
    return subItems();
}

bool Group::configureImpl() {
    VPSS_GRP_ATTR_S *attrs = attributes()->createAttributes();

    // создаём группу
    if (HI_MPI_VPSS_CreateGrp(id(), attrs) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VPSS_CreateGrp failed");

    if (attributes<GroupParameters>() != nullptr)
        attributes<GroupParameters>()->setAttributes();

    // стартуем каналы
    if (!Configurator::configureImpl())
        return false;

    // стартуем группу
    if (HI_MPI_VPSS_StartGrp(id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VPSS_StartGrp failed");

    return true;
}

void Group::setBindedSource(BindItem *bi) {
    BindItem::setBindedSource(bi);
    m_source = bindedSource<IVideoCaptureFormatSource>();

    if (m_source == nullptr)
        throw std::runtime_error("bindedSource must realize IVideoCaptureFormatSource");
}

SIZE_S Group::imgSize() const {
    return m_source->imgSize();
}

HI_U32 Group::fps() const {
    return m_source->fps();
}

PIXEL_FORMAT_E Group::pixelFormat() const {
    return m_source->pixelFormat();
}

IVideoCaptureFormatSource *Group::source() const {
    if (m_source == nullptr)
        throw std::runtime_error("m_source is not set");

    return m_source;
}

}
