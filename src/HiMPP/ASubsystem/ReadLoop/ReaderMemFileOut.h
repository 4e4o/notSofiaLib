#ifndef READER_MEMORY_FILE_OUT_H
#define READER_MEMORY_FILE_OUT_H

#include "ReaderMemOut.h"
#include "ReaderFileOut.h"

#include <memory>

namespace hisilicon::mpp {

class ReaderMemFileOut : public ReaderMemOut {
  public:
    ReaderMemFileOut(const std::string &fname, size_t s)
        : ReaderMemOut(s), m_out(new ReaderFileOut(fname)) {
    }

    ~ReaderMemFileOut() {
        const auto &buf = buffer();
        m_out->write(buf.data(), buf.size());
    }

  private:
    std::unique_ptr<IReaderOut> m_out;
    std::ofstream m_file;

};

}

#endif // READER_MEMORY_FILE_OUT_H

