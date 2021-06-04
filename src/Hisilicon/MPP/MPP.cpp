#include "MPP.h"
#include "VI/Source/InfoProvider.h"
#include "VI/Subsystem.h"
#include "VideoBuffer.h"
#include "ElementsFactory.h"

#include <stdexcept>

#include <mpi_sys.h>

// TODO recommended value is 16
// page 90 of
// HiMPP Media Processing Software Development Reference.pdf
#define DEFAULT_SYS_WIDTH_ALIGN 64

namespace hisilicon {
namespace mpp {

MPP::MPP(vi::InfoProvider* v)
    : m_sourceViInfo(v),
      m_sysWidthAlign(DEFAULT_SYS_WIDTH_ALIGN),
      m_factory(new ElementsFactory()) {
}

MPP::~MPP() {
    HI_MPI_SYS_Exit();
}

bool MPP::configureImpl() {
    m_vi.reset(m_factory->vi(this));
    // сначала настраиваем видео пул
    m_videoBuffer.reset(m_factory->videoBuffer(this));
    init();
    m_vi->configure();
    return true;
}

void MPP::init() {
    MPP_SYS_CONF_S stSysConf{};
    stSysConf.u32AlignWidth = sysWidthAlign();

    if (HI_MPI_SYS_SetConf(&stSysConf) != HI_SUCCESS)
        throw "HI_MPI_SYS_SetConf failed";

    if (HI_MPI_SYS_Init() != HI_SUCCESS)
        throw "HI_MPI_SYS_Init failed";
}

ElementsFactory* MPP::factory() const {
    return m_factory.get();
}

void MPP::setFactory(ElementsFactory* factory) {
    m_factory.reset(factory);
}

vi::InfoProvider* MPP::viSourceInfo() const {
    return m_sourceViInfo.get();
}

void MPP::setSysWidthAlign(HI_U32 sa) {
    // resrictions:
    // HiMPP Media Processing Software Development Reference.pdf
    // page 90
    if ((sa < 1) || (sa > 1024))
        throw std::runtime_error("sys width align must be in [1:1024]");

    if ((sa > 1) && ((sa % 16) != 0))
        throw std::runtime_error("sys width align > 1 must be multiple of 16");

    m_sysWidthAlign = sa;
}

HI_U32 MPP::sysWidthAlign() const {
    return m_sysWidthAlign;
}

vi::Subsystem* MPP::vi() const {
    return m_vi.get();
}

}
}
