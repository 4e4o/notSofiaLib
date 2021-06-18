#ifndef I_READER_OUT_H
#define I_READER_OUT_H

#include <hi_common.h>

namespace hisilicon::mpp {

class IReaderOut {
  public:
    IReaderOut() { }
    virtual ~IReaderOut() { }

    virtual void write(const HI_U8 *, const HI_U32 &) = 0;
};

}

#endif // I_READER_OUT_H

