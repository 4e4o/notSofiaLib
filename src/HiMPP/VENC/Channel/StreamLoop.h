#ifndef STREAM_LOOP_H
#define STREAM_LOOP_H

#include <atomic>
#include <functional>
#include <vector>
#include <memory>

namespace hisilicon::mpp::venc {

class StreamBuffer;

class StreamLoop {
  public:
    typedef std::function<void(StreamBuffer *)> Event;

    StreamLoop();
    ~StreamLoop();

    template<class Callable>
    void addFd(int fd, Callable &&c) {
        if (m_running.load())
            throw std::runtime_error("You must add fds before calling run");

        m_fds.push_back(fd);
        m_events.emplace_back(std::move(c));
    }

    void setTimeoutSec(int);

    void run();

    void stop();

  private:
    int maxFd() const;
    void createInterruptPipe();

    int m_interruptPipe[2];
    std::atomic<bool> m_running;
    fd_set m_readFds;
    std::vector<int> m_fds;
    std::vector<Event> m_events;
    int m_timeoutSec;
    std::unique_ptr<StreamBuffer> m_buffer;
};

}

#endif // STREAM_LOOP_H

