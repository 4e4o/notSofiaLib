#include "ReaderFileOut.h"

namespace hisilicon::mpp {

using namespace std;

ReaderFileOut::ReaderFileOut(const std::string &fname) {
    m_file.open(fname, ios::binary | ios::trunc);

    if (!m_file.is_open())
        throw runtime_error("Can't open output file");
}

ReaderFileOut::~ReaderFileOut() {
}

void ReaderFileOut::write(const HI_U8 *data, const HI_U32 &size) {
    m_file.write(reinterpret_cast<const char *>(data), size);
}

}
