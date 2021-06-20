#ifndef MOTION_READER_H
#define MOTION_READER_H

#include "HiMPP/VDA/Channel/VdaDataReader.h"

namespace hisilicon::mpp::vda {

class MotionReader : public VdaDataReader {
  public:
    using VdaDataReader::VdaDataReader;

  private:
    void allocate(const VDA_CHN_STAT_S &, VDA_DATA_S &) override final;
    void onData(const VDA_DATA_S &) override final;
    DataBufferWrapper *createBufferWrapper(DataBuffer *) override final;
};

}

#endif // MOTION_READER_H

