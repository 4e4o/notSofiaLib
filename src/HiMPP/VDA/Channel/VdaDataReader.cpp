#include "VdaDataReader.h"

#include <mpi_vda.h>

namespace hisilicon::mpp::vda {

void VdaDataReader::attach(ReadLoop *loop) {
    const HI_S32 fd = HI_MPI_VDA_GetFd(id());
    LoopReader::attach(fd, loop);
}

void VdaDataReader::read() {
    VDA_CHN_STAT_S stStat{};
    VDA_DATA_S vdaData;

    if (HI_MPI_VDA_Query(id(), &stStat) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VDA_Query failed");

    allocate(stStat, vdaData);

    const HI_S32 r = HI_MPI_VDA_GetData(id(), &vdaData, HI_FALSE);

    if (r == HI_ERR_VDA_BUF_EMPTY)
        return;

    if (r != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VDA_GetData failed");

    onData(vdaData);

    if (HI_MPI_VDA_ReleaseData(id(), &vdaData) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VDA_ReleaseData failed");
}

}
