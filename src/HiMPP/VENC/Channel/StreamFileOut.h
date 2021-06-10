#ifndef STREAM_FILE_OUT_H
#define STREAM_FILE_OUT_H

#include "IStreamOut.h"

#include <fstream>

namespace hisilicon::mpp::venc {

class Channel;

class StreamFileOut : public IStreamOut {
public:
    StreamFileOut(Channel*);
    ~StreamFileOut();

private:
    void write(const HI_U8*, const HI_U32&) override final;

    Channel* m_channel;
    std::ofstream m_file;
};

}

#endif // STREAM_FILE_OUT_H

