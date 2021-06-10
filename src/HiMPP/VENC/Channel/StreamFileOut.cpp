#include "StreamFileOut.h"
#include "Channel.h"
#include "StreamReader.h"

namespace hisilicon::mpp::venc {

using namespace std;

StreamFileOut::StreamFileOut(Channel *c) :
    m_channel(c) {
    const string ext = c->h264Mode() ? ".h264" : "";
    const string fname = "stream_" + to_string(m_channel->id()) + ext;
    m_file.open(fname, ios::binary | ios::trunc);

    if (!m_file.is_open())
        throw runtime_error("Can't open output file");
}

StreamFileOut::~StreamFileOut() {
}

void StreamFileOut::write(const HI_U8* data, const HI_U32& size) {
    m_file.write(reinterpret_cast<const char*>(data), size);
}

}
