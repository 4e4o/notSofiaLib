#include "Group.h"
#include "Channel.h"
#include "HiMPP/MPP.h"
#include "Subsystem.h"
#include "IGroupSource.h"

#include <stdexcept>
#include <iostream>

#include <mpi_vpss.h>

namespace hisilicon::mpp::vpss {

static VPSS_GRP_ATTR_S *createStandardAttr() {
    return new VPSS_GRP_ATTR_S{};
}

Group::Group(Subsystem *s, int id)
    : ASubsystemItem(s, id),
      VpssBindItem(this),
      m_attrs(createStandardAttr()),
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
    if (m_source == nullptr)
        throw std::runtime_error("vi::Group source is not set");

    if (m_attrs.get() == nullptr)
        throw std::runtime_error("vi::Group attributes not set");

    // создаём группу
    if (HI_MPI_VPSS_CreateGrp(id(), m_attrs.get()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VPSS_CreateGrp failed");

    if (m_params.get() != nullptr) {
        // устанавливаем параметры
        if (HI_MPI_VPSS_SetGrpParam(id(), m_params.get()) != HI_SUCCESS)
            throw std::runtime_error("HI_MPI_VPSS_SetGrpParam failed");
    }

    // стартуем каналы
    if (!Configurator::configureImpl())
        return false;

    // стартуем группу
    if (HI_MPI_VPSS_StartGrp(id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VPSS_StartGrp failed");

    return true;
}

void Group::setAttributes(VPSS_GRP_ATTR_S *attr) {
    m_attrs.reset(attr);
}

void Group::setSource(IGroupSource *s) {
    m_source = s;

    const SIZE_S size = s->destSize();
    m_attrs->enPixFmt = s->pixelFormat();
    m_attrs->u32MaxW = size.u32Width;
    m_attrs->u32MaxH = size.u32Height;
}

SIZE_S Group::imgSize() const {
    return m_source->imgSize();
}

bool Group::pal() const {
    return m_source->pal();
}

PIXEL_FORMAT_E Group::pixelFormat() const {
    return m_source->pixelFormat();
}

void Group::setParameters(VPSS_GRP_PARAM_S *p) {
    m_params.reset(p);
}

}
