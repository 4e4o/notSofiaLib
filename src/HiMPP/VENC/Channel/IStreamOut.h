#ifndef I_STREAM_OUT_H
#define I_STREAM_OUT_H

#include <hi_common.h>

namespace hisilicon::mpp::venc {

class IStreamOut {
public:
    IStreamOut() { }
    virtual ~IStreamOut() { }

    virtual void write(const HI_U8*, const HI_U32&) = 0;
};

}

#endif // I_STREAM_OUT_H

