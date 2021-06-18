#ifndef LOOP_READER_H
#define LOOP_READER_H

#include <memory>

#include "Misc/IdHolder.h"
#include "ReadLoop.h"
#include "IReaderOut.h"

namespace hisilicon::mpp {

template<class DataBufferWrapper>
class LoopReader : public Holder<IdHolder *> {
  public:
    void setOut(IReaderOut *out, bool getOwnership = true) {
        releaseOut();
        m_out.reset(out);
        m_ownsOut = getOwnership;
    }

  protected:
    LoopReader(IdHolder *h)
        : Holder(h), m_ownsOut(true) { }

    virtual ~LoopReader() {
        releaseOut();
    }

    void attach(int fd, ReadLoop *loop) {
        if (fd < 0)
            throw std::runtime_error("[LoopReader] invalid fd");

        m_buffer.reset(new DataBufferWrapper(loop->buffer()));

        loop->addFd(fd, [this] () {
            read();
        });
    }

    DataBufferWrapper *buffer() const {
        return m_buffer.get();
    }

    int id() const {
        return Holder::value()->id();
    }

    void write(const HI_U8 *data, const HI_U32 &size) {
        m_out->write(data, size);
    }

  private:
    virtual void read() = 0;

    void releaseOut() {
        if (!m_ownsOut)
            m_out.release();
    }

    std::unique_ptr<DataBufferWrapper> m_buffer;
    std::unique_ptr<IReaderOut> m_out;
    bool m_ownsOut;
};

}

#endif // LOOP_READER_H

