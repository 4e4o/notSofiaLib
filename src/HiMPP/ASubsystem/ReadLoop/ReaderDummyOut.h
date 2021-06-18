#ifndef READER_DUMMY_OUT_H
#define READER_DUMMY_OUT_H

#include "IReaderOut.h"

namespace hisilicon::mpp {

class ReaderDummyOut : public IReaderOut {
  public:
    ReaderDummyOut() { }
    ~ReaderDummyOut() { }

  private:
    void write(const HI_U8 *, const HI_U32 &) override final { }
};

}

#endif // READER_DUMMY_OUT_H

