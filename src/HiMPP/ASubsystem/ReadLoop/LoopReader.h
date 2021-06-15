#ifndef LOOP_READER_H
#define LOOP_READER_H

#include <memory>

#include "Misc/IdHolder.h"
#include "ReadLoop.h"

namespace hisilicon::mpp {

template<class DataBufferWrapper>
class LoopReader : public Holder<IdHolder *> {
  protected:
    LoopReader(IdHolder *h)
        : Holder(h) { }

    virtual ~LoopReader() { }

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

  private:
    virtual void read() = 0;

    std::unique_ptr<DataBufferWrapper> m_buffer;
};

}

#endif // LOOP_READER_H

