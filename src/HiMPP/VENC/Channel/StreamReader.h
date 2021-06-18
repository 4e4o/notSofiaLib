#ifndef STREAM_READER_H
#define STREAM_READER_H

#include "HiMPP/ASubsystem/ReadLoop/LoopReader.h"
#include "StreamBuffer.h"

namespace hisilicon::mpp::venc {

class StreamReader : public LoopReader<StreamBuffer> {
  public:
    StreamReader(IdHolder *);

    void attach(ReadLoop *);
    void setConsecutiveMode(bool consecutiveMode);

  private:
    void read() override final;

    bool m_consecutiveMode;
};

}

#endif // STREAM_READER_H

