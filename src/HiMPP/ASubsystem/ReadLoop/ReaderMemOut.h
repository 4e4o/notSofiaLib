#ifndef READER_MEMORY_OUT_H
#define READER_MEMORY_OUT_H

#include "IReaderOut.h"

#include <span>

namespace hisilicon::mpp {

class ReaderMemOut : public IReaderOut {
  public:
    ReaderMemOut(size_t s);
    ~ReaderMemOut();

  protected:
    std::span<HI_U8> buffer() const;

  private:
    void write(const HI_U8 *data, const HI_U32 &size) override final;

    std::span<HI_U8> m_buffer;
    size_t m_pos;
};

}

#endif // READER_MEMORY_OUT_H

