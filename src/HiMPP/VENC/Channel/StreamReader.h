#ifndef STREAM_READER_H
#define STREAM_READER_H

#include <functional>
#include <memory>

#include <hi_comm_venc.h>

namespace hisilicon::mpp {
class ReadLoop;
}

namespace hisilicon::mpp::venc {

class Channel;
class StreamBuffer;

class StreamReader {
  public:
    typedef std::function<void(const HI_U8 *, const HI_U32 &)> Event;

    StreamReader(Channel *);
    ~StreamReader();

    void attach(ReadLoop *);

    template<class Callable>
    void setEvent(Callable &&c) {
        m_event = std::move(c);
    }

    void setConsecutiveMode(bool consecutiveMode);

  private:
    void read();

    Channel *m_channel;
    Event m_event;
    bool m_consecutiveMode;
    std::unique_ptr<StreamBuffer> m_buffer;
};

}

#endif // STREAM_READER_H

