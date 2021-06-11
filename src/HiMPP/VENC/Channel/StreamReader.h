#ifndef STREAM_READER_H
#define STREAM_READER_H

#include <functional>

#include <hi_comm_venc.h>

namespace hisilicon::mpp::venc {

class Channel;
class StreamBuffer;
class StreamLoop;

class StreamReader {
  public:
    typedef std::function<void(const HI_U8 *, const HI_U32 &)> Event;

    StreamReader(Channel *);
    ~StreamReader();

    void attach(StreamLoop *);

    template<class Callable>
    void setEvent(Callable &&c) {
        m_event = std::move(c);
    }

  private:
    void read(StreamBuffer *buffer);

    Channel *m_channel;
    Event m_event;
};

}

#endif // STREAM_READER_H

