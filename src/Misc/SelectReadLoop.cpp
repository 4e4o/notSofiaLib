#include "SelectReadLoop.h"

#include <iostream>

#define DEFAULT_TIMEOUT_MILLIS 2000

SelectReadLoop::SelectReadLoop() :
    m_timeout(DEFAULT_TIMEOUT_MILLIS) {
    createInterruptPipe();
}

SelectReadLoop::~SelectReadLoop() {
    ::close(m_interruptPipe[0]);
    ::close(m_interruptPipe[1]);
}

void SelectReadLoop::createInterruptPipe() {
    if (::pipe(m_interruptPipe) != 0)
        throw std::runtime_error("Create pipe failed");

    addFd(m_interruptPipe[0], [] () {});
}

int SelectReadLoop::maxFd() const {
    int max = 0;

    for (int fd : m_fds) {
        if (fd > max)
            max = fd;
    }

    return max;
}

void SelectReadLoop::setTimeout(const std::chrono::milliseconds &t) {
    m_timeout = t;
}

void SelectReadLoop::onTimeout() {
}

void SelectReadLoop::run() {
    struct timeval timeout;

    m_running.store(true);

    while (m_running.load()) {
        FD_ZERO(&m_readFds);

        for (int fd : m_fds) {
            FD_SET(fd, &m_readFds);
        }

        {
            using namespace std::chrono;
            auto sec = duration_cast<seconds>(m_timeout);
            timeout.tv_sec = sec.count();
            timeout.tv_usec = (duration_cast<nanoseconds>(m_timeout) -
                               duration_cast<nanoseconds>(sec)).count();
        }

        const int result = select(maxFd() + 1, &m_readFds, nullptr, nullptr, &timeout);

        if (result < 0) {
            if (errno != EINTR) {
                std::cout << "[ReadLoop] select failed!" << std::endl;
                m_running.store(false);
            }

            continue;
        } else if (result == 0) {
            onTimeout();
            continue;
        } else {
            for (int i = 0 ; i < (int) m_fds.size() ; i++) {
                if (FD_ISSET(m_fds[i], &m_readFds)) {
                    // если interrupt pipe
                    if (i == 0)
                        break;

                    m_events[i]();
                }
            }
        }
    }
}

void SelectReadLoop::stop() {
    m_running.store(false);

    // это чтоб селект сразу разблокировался если заблокирован
    char byte = 1;
    ::write(m_interruptPipe[1], &byte, 1);
}

