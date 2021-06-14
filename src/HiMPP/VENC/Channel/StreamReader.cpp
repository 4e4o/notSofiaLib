#include "StreamReader.h"
#include "HiMPP/ASubsystem/ReadLoop/ReadLoop.h"
#include "HiMPP/ASubsystem/ReadLoop/DataBuffer.h"
#include "Channel.h"
#include "StreamBuffer.h"

#include <mpi_venc.h>

namespace hisilicon::mpp::venc {

StreamReader::StreamReader(Channel *c)
    : m_channel(c),
      m_consecutiveMode(false) {
}

StreamReader::~StreamReader() {
}

void StreamReader::attach(ReadLoop *loop) {
    const HI_S32 fd = HI_MPI_VENC_GetFd(m_channel->id());

    if (fd < 0)
        throw std::runtime_error("HI_MPI_VENC_GetFd failed");

    m_buffer.reset(new StreamBuffer(loop->buffer()));

    loop->addFd(fd, [this] () {
        read();
    });
}

void StreamReader::read() {
    VENC_CHN_STAT_S stStat{};
    VENC_STREAM_S stStream;

    if (HI_MPI_VENC_Query(m_channel->id(), &stStat) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_Query failed");

    stStream.pstPack = m_buffer->getPackBuffer(stStat.u32CurPacks);
    stStream.u32PackCount = stStat.u32CurPacks;

    if (stStream.pstPack == nullptr)
        throw std::runtime_error("StreamReader::read buffer is nullptr");

    const HI_S32 streamResult = HI_MPI_VENC_GetStream(m_channel->id(), &stStream,
                                HI_FALSE);

    if (streamResult == HI_ERR_VENC_BUF_EMPTY)
        return;

    if (streamResult != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_GetStream failed");

    for (HI_U32 i = 0; i < stStream.u32PackCount; i++) {
        if (stStream.pstPack[i].u32Len[1] > 0) {
            HI_U8 *data;

            if (m_consecutiveMode)
                data = stStream.pstPack[i].pu8Addr[0];
            else
                data = m_buffer->getConsecutiveStreamBuffer(stStream.pstPack[i]);

            m_event(data, stStream.pstPack[i].u32Len[0] + stStream.pstPack[i].u32Len[1]);
        } else {
            m_event(stStream.pstPack[i].pu8Addr[0], stStream.pstPack[i].u32Len[0]);
        }
    }

    if (HI_MPI_VENC_ReleaseStream(m_channel->id(), &stStream) != HI_SUCCESS)
        throw std::runtime_error("HI_MPI_VENC_ReleaseStream failed");
}

void StreamReader::setConsecutiveMode(bool consecutiveMode) {
    // HiMPP Media Processing Software Development Reference.pdf
    // page 510
    m_consecutiveMode = consecutiveMode;
}

}
