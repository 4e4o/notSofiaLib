#include "StreamReader.h"
#include "StreamBuffer.h"
#include "StreamLoop.h"
#include "Channel.h"

#include <mpi_venc.h>

namespace hisilicon::mpp::venc {

StreamReader::StreamReader(Channel *c)
    : m_channel(c) {
}

StreamReader::~StreamReader() {
}

void StreamReader::attach(StreamLoop *loop) {
    const HI_S32 fd = HI_MPI_VENC_GetFd(m_channel->id());

    if (fd < 0)
        throw std::runtime_error("HI_MPI_VENC_GetFd failed");

    loop->addFd(fd, [this] (StreamBuffer * buffer) {
        read(buffer);
    });
}

void StreamReader::read(StreamBuffer *buffer) {
    VENC_CHN_STAT_S stStat{};
    VENC_STREAM_S stStream;

    if (HI_MPI_VENC_Query(m_channel->id(), &stStat) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_Query failed");

    stStream.pstPack = buffer->get(stStat.u32CurPacks);
    stStream.u32PackCount = stStat.u32CurPacks;

    if (nullptr == stStream.pstPack)
        throw std::runtime_error("StreamReader::read buffer is nullptr");

    if (HI_MPI_VENC_GetStream(m_channel->id(), &stStream, HI_TRUE) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_GetStream failed");

    for (HI_U32 i = 0; i < stStream.u32PackCount; i++) {
        m_event(stStream.pstPack[i].pu8Addr[0], stStream.pstPack[i].u32Len[0]);

        if (stStream.pstPack[i].u32Len[1] > 0)
            m_event(stStream.pstPack[i].pu8Addr[1], stStream.pstPack[i].u32Len[1]);
    }

    if (HI_MPI_VENC_ReleaseStream(m_channel->id(), &stStream) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_ReleaseStream failed");
}

}
