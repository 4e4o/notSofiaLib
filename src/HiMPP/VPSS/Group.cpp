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
    VPSS_GRP_ATTR_S *attr = new VPSS_GRP_ATTR_S{};

    attr->bDrEn = HI_FALSE;
    attr->bDbEn = HI_FALSE;
    attr->bIeEn = HI_FALSE;
    attr->bNrEn = HI_TRUE;
    attr->bHistEn = HI_FALSE;
    attr->enDieMode = VPSS_DIE_MODE_AUTO;

    return attr;
}

Group::Group(Subsystem *p, int id)
    : Holder<Subsystem*>(p), IdHolder(id),
      m_attrs(createStandardAttr()),
      m_source(nullptr) {
}

Group::~Group() {
    // сначала останавливаем группу
    HI_MPI_VPSS_StopGrp(id());

    // потом каналы
    Configurator::clear();

    // потом удаляем группу
    HI_MPI_VPSS_DestroyGrp(id());
    std::cout << "~vpss::Group " << this << " , " << id() << std::endl;
}

Subsystem *Group::subsystem() const {
    return Holder<Subsystem *>::value();
}

HI_S32 Group::receiverBindDeviceId() {
    return id();
}

HI_S32 Group::receiverBindChannelId() {
    return 0;
}

HI_S32 Group::sourceBindDeviceId() {
    return id();
}

HI_S32 Group::sourceBindChannelId() {
    return 0;
}

Channel *Group::addChannel(int id) {
    Channel *ch = subsystem()->parent()->create<Channel>(this, id);
    addItemBack(ch);
    m_channels.push_back(ch);
    return ch;
}

bool Group::configureImpl() {
    if (m_source == nullptr)
        throw std::runtime_error("vi::Group source is not set");

    if (m_attrs.get() == nullptr)
        throw std::runtime_error("vi::Group attributes not set");

    return Configurator::configureImpl();
}

bool Group::startImpl() {
    // создаём группу
    if (HI_MPI_VPSS_CreateGrp(id(), m_attrs.get()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VPSS_CreateGrp failed");

    if (m_params.get() != nullptr) {
        // устанавливаем параметры
        if (HI_MPI_VPSS_SetGrpParam(id(), m_params.get()) != HI_SUCCESS)
            throw std::runtime_error("HI_MPI_VPSS_SetGrpParam failed");
    }

    // стартуем каналы
    if (!Configurator::startImpl())
        return false;

    // стартуем группу
    if (HI_MPI_VPSS_StartGrp(id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VPSS_StartGrp failed");

    return true;
}

const std::vector<Channel *> &Group::channels() const {
    return m_channels;
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

// venc::IChannelSource
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
