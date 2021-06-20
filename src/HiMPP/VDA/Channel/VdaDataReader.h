#ifndef VDA_DATA_READER_H
#define VDA_DATA_READER_H

#include "HiMPP/ASubsystem/ReadLoop/LoopReader.h"

#include <hi_comm_vda.h>

namespace hisilicon::mpp::vda {

class VdaDataReader : public LoopReader {
  public:
    using LoopReader::LoopReader;

    void attach(ReadLoop *loop);

  private:
    virtual void allocate(const VDA_CHN_STAT_S &, VDA_DATA_S &) = 0;
    virtual void onData(const VDA_DATA_S &) = 0;

    void read() override final;
};

}

#endif // VDA_DATA_READER_H
