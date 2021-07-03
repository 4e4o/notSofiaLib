#include "Timer.h"

#include <unistd.h>
#include <sys/timerfd.h>

#include <stdexcept>

Timer::Timer(EpollLoop *l)
    : Holder<EpollLoop *>(l),
      m_fd(INVALID_FD),
      m_oneShot(false) {
    open();
    attachToLoop(value(), EpoolOperationType{.READ = 1}, m_fd);
}

Timer::~Timer() {
    removeFromLoop(m_fd);
    close();
}

static void setTimespec(timespec &s, const std::chrono::milliseconds &t) {
    using namespace std::chrono;
    auto sec = duration_cast<seconds>(t);
    auto nano = duration_cast<nanoseconds>(t);
    s.tv_sec = sec.count();
    s.tv_nsec = duration_cast<nanoseconds>(nano - sec).count();
}

bool Timer::start(const std::chrono::milliseconds &t) {
    itimerspec ts{};

    if (!m_oneShot)
        setTimespec(ts.it_interval, t);

    setTimespec(ts.it_value, t);

    if (timerfd_settime(m_fd, 0, &ts, nullptr) < 0)
        throw std::runtime_error("timerfd_settime failed");

    return true;
}

bool Timer::stop() {
    itimerspec ts{};

    if (timerfd_settime(m_fd, 0, &ts, nullptr) < 0)
        throw std::runtime_error("timerfd_settime failed");

    return true;
}

void Timer::onRead() {
    uint64_t res;
    ::read(m_fd, &res, sizeof(res));

    if (m_tick)
        m_tick(this);
}

void Timer::open() {
    m_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);

    if (m_fd == INVALID_FD)
        throw std::runtime_error("timerfd_create failed");
}

void Timer::close() {
    if (m_fd == INVALID_FD)
        return;

    ::close(m_fd);
}

bool Timer::oneShot() const {
    return m_oneShot;
}

void Timer::setOneShot(bool oneShot) {
    m_oneShot = oneShot;
}
