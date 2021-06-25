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
      m_attrBuilder(new GroupAttributes()),
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

    if (m_attrBuilder.get() == nullptr)
        throw std::runtime_error("vi::Group attributes builder is null");

    std::unique_ptr<VPSS_GRP_ATTR_S> attrs(m_attrBuilder->build(m_source));

    if (attrs.get() == nullptr)
        throw std::runtime_error("vi::Group attributes is null");

    // создаём группу
    if (HI_MPI_VPSS_CreateGrp(id(), attrs.get()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VPSS_CreateGrp failed");

    if (m_paramsBuilder.get() != nullptr) {
        std::unique_ptr<VPSS_GRP_PARAM_S> params(m_paramsBuilder->build());

        if (params.get() != nullptr) {
            // устанавливаем параметры
            if (HI_MPI_VPSS_SetGrpParam(id(), params.get()) != HI_SUCCESS)
                throw std::runtime_error("HI_MPI_VPSS_SetGrpParam failed");
        }
    }

    // стартуем каналы
    if (!Configurator::configureImpl())
        return false;

    // стартуем группу
    if (HI_MPI_VPSS_StartGrp(id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VPSS_StartGrp failed");

    return true;
}

void Group::setAttributes(GroupAttributes *builder) {
    m_attrBuilder.reset(builder);
}

void Group::setParameters(GroupParameters *p) {
    m_paramsBuilder.reset(p);
}

GroupAttributes *Group::attributes() const {
    return m_attrBuilder.get();
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

}
