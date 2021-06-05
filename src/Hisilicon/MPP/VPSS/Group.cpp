#include "Group.h"
#include "Channel.h"
#include "Hisilicon/MPP/MPP.h"
#include "Hisilicon/MPP/ElementsFactory.h"
#include "Subsystem.h"

#include <stdexcept>
#include <iostream>

#include <mpi_vpss.h>

namespace hisilicon {
namespace mpp {
namespace vpss {

Group::Group(Subsystem* p, int id)
    : Holder<Subsystem*>(p), IdHolder(id) {
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

Subsystem* Group::subsystem() const {
    return Holder<Subsystem*>::value();
}

Channel* Group::addChannel(int id) {
    Channel* ch = subsystem()->parent()->factory()->vpssChannel(this, id);
    addItem(ch);
    m_channels.push_back(ch);
    return ch;
}

bool Group::configureImpl() {
    if (m_attrs.get() == NULL)
        throw std::runtime_error("vi::Group attributes not set");

    if (m_params.get() == NULL)
        throw std::runtime_error("vi::Group parameters not set");

    return Configurator::configureImpl();
}

bool Group::startImpl() {
    // создаём группу
    if (HI_MPI_VPSS_CreateGrp(id(), m_attrs.get()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VPSS_CreateGrp failed");

    // устанавливаем параметры
    if (HI_MPI_VPSS_SetGrpParam(id(), m_params.get()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VPSS_SetGrpParam failed");

    // стартуем каналы
    if (!Configurator::startImpl())
        return false;

    // стартуем группу
    if (HI_MPI_VPSS_StartGrp(id()) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VPSS_StartGrp failed");

    return true;
}

const std::vector<Channel*>& Group::channels() const {
    return m_channels;
}

void Group::setAttributes(VPSS_GRP_ATTR_S* attr) {
    m_attrs.reset(attr);
}

void Group::setParameters(VPSS_GRP_PARAM_S* p) {
    m_params.reset(p);
}

}
}
}
