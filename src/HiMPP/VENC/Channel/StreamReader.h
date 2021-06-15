#ifndef STREAM_READER_H
#define STREAM_READER_H

#include <functional>

#include <hi_comm_venc.h>

#include "HiMPP/ASubsystem/ReadLoop/LoopReader.h"
#include "StreamBuffer.h"

namespace hisilicon::mpp::venc {

class StreamReader : public LoopReader<StreamBuffer> {
  public:
    typedef std::function<void(const HI_U8 *, const HI_U32 &)> Event;

    StreamReader(IdHolder *);

    void attach(ReadLoop *);

    template<class Callable>
    void setEvent(Callable &&c) {
        m_event = std::move(c);
    }

    void setConsecutiveMode(bool consecutiveMode);

  private:
    void read() override final;

    Event m_event;
    bool m_consecutiveMode;
};

}

#endif // STREAM_READER_H

