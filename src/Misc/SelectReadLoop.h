#ifndef SELECT_READ_LOOP_H
#define SELECT_READ_LOOP_H

#include <atomic>
#include <functional>
#include <vector>
#include <chrono>

class SelectReadLoop {
  public:
    typedef std::function<void()> Event;

    SelectReadLoop();
    ~SelectReadLoop();

    template<class Callable>
    void addFd(int fd, Callable &&c) {
        if (m_running.load())
            throw std::runtime_error("You must add fds before calling run");

        m_fds.push_back(fd);
        m_events.emplace_back(std::move(c));
    }

    void setTimeout(const std::chrono::milliseconds & );

    void run();
    void stop();

  protected:
    virtual void onTimeout();

  private:
    int maxFd() const;
    void createInterruptPipe();

    int m_interruptPipe[2];
    std::atomic<bool> m_running;
    fd_set m_readFds;
    std::vector<int> m_fds;
    std::vector<Event> m_events;
    std::chrono::milliseconds m_timeout;
};

#endif // SELECT_READ_LOOP_H
