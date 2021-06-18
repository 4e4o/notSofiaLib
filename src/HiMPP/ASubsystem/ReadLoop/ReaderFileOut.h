#ifndef READER_FILE_OUT_H
#define READER_FILE_OUT_H

#include "IReaderOut.h"

#include <fstream>

namespace hisilicon::mpp {

class ReaderFileOut : public IReaderOut {
  public:
    ReaderFileOut(const std::string &);
    ~ReaderFileOut();

  private:
    void write(const HI_U8 *, const HI_U32 &) override final;

    std::ofstream m_file;
};

}

#endif // READER_FILE_OUT_H

