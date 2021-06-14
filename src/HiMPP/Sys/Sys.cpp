#include "Sys.h"

#include <stdexcept>
#include <iostream>

#include <mpi_sys.h>

// HiMPP Media Processing Software Development Reference.pdf
// page 90
#define DEFAULT_SYS_WIDTH_ALIGN 16

namespace hisilicon::mpp {

Sys::Sys(MPP *p)
    : MPPChild(p),
      m_sysWidthAlign(DEFAULT_SYS_WIDTH_ALIGN) {
}

Sys::~Sys() {
    HI_MPI_SYS_Exit();
    std::cout << "~Sys " << this << std::endl;
}

bool Sys::configureImpl() {
    MPP_SYS_CONF_S stSysConf{};
    stSysConf.u32AlignWidth = sysWidthAlign();

    if (HI_MPI_SYS_SetConf(&stSysConf) != HI_SUCCESS)
        throw "HI_MPI_SYS_SetConf failed";

    if (HI_MPI_SYS_Init() != HI_SUCCESS)
        throw "HI_MPI_SYS_Init failed";

    return true;
}

void Sys::setSysWidthAlign(HI_U32 sa) {
    // restrictions:
    // HiMPP Media Processing Software Development Reference.pdf
    // page 90
    if ((sa < 1) || (sa > 1024))
        throw std::runtime_error("sys width align must be in [1:1024]");

    if ((sa > 1) && ((sa % 16) != 0))
        throw std::runtime_error("sys width align > 1 must be multiple of 16");

    m_sysWidthAlign = sa;
}

HI_U32 Sys::sysWidthAlign() const {
    return m_sysWidthAlign;
}

}
