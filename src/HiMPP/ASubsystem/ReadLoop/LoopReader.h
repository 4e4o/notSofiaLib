#ifndef LOOP_READER_H
#define LOOP_READER_H

#include <memory>

#include <hi_common.h>

#include "Misc/IdHolder.h"
#include "Misc/EventLoop/Epollable.h"

namespace hisilicon::mpp {

class DataBuffer;
class IReaderOut;
class ReadLoop;
class DataBufferWrapper;

class LoopReader : public Holder<IdHolder *>, public Epollable {
  public:
    LoopReader(IdHolder *h);
    virtual ~LoopReader();

    void setOut(IReaderOut *out, bool getOwnership = true);

  protected:

    void attach(int fd, ReadLoop *loop);

    template<class TBuffer = DataBufferWrapper>
    TBuffer * buffer() const {
        return static_cast<TBuffer *>(m_buffer.get());
    }

    int id() const;
    void write(const HI_U8 *data, const HI_U32 &size);

  private:
    virtual DataBufferWrapper *createBufferWrapper(DataBuffer *) = 0;

    void releaseOut();

    std::unique_ptr<DataBufferWrapper> m_buffer;
    std::unique_ptr<IReaderOut> m_out;
    bool m_ownsOut;
    int m_fd;
};

}

#endif // LOOP_READER_H

