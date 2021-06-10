#ifndef STREAM_DUMMY_OUT_H
#define STREAM_DUMMY_OUT_H

#include "IStreamOut.h"

namespace hisilicon::mpp::venc {

class StreamDummyOut : public IStreamOut {
public:
    StreamDummyOut() { }
    ~StreamDummyOut() { }

private:
    void write(const HI_U8*, const HI_U32&) override final { }
};

}

#endif // STREAM_DUMMY_OUT_H

