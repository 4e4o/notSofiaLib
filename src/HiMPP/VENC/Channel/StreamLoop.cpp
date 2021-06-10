#include "StreamLoop.h"
#include "StreamBuffer.h"

#include <iostream>

#define DEFAULT_TIMEOUT_SEC 2

namespace hisilicon::mpp::venc {

StreamLoop::StreamLoop() :
    m_timeoutSec(DEFAULT_TIMEOUT_SEC),
    m_buffer(new StreamBuffer()) {
    createInterruptPipe();
}

StreamLoop::~StreamLoop() {
    ::close(m_interruptPipe[1]);

    for (int fd : m_fds)
        ::close(fd);
}

void StreamLoop::createInterruptPipe() {
    if (::pipe(m_interruptPipe) != 0)
        throw std::runtime_error("Create pipe failed");

    addFd(m_interruptPipe[0], [] (StreamBuffer*) {});
}

int StreamLoop::maxFd() const {
    int max = 0;

    for (int fd : m_fds) {
        if (fd > max)
            max = fd;
    }

    return max;
}

void StreamLoop::setTimeoutSec(int t) {
    m_timeoutSec = t;
}

void StreamLoop::run() {
    struct timeval timeout;

    m_running.store(true);

    while (m_running.load()) {
        FD_ZERO(&m_readFds);

        for (int fd : m_fds) {
            FD_SET(fd, &m_readFds);
        }

        timeout.tv_sec = m_timeoutSec;
        timeout.tv_usec = 0;

        const int result = select(maxFd() + 1, &m_readFds, nullptr, nullptr, &timeout);

        if (result < 0) {
            if (errno != EINTR) {
                std::cout << "[StreamLoop] select failed!" << std::endl;
                m_running.store(false);
            }

            continue;
        } else if (result == 0) {
            std::cout << "[StreamLoop] select timeout, no data" << std::endl;
            continue;
        } else {
            for (int i = 0 ; i < (int) m_fds.size() ; i++) {
                if (FD_ISSET(m_fds[i], &m_readFds)) {
                    // если interrupt pipe
                    if (i == 0)
                        break;

                    m_events[i](m_buffer.get());
                }
            }
        }
    }
}

void StreamLoop::stop() {
    m_running.store(false);

    // это чтоб селект сразу разблокировался если заблокирован
    char byte = 1;
    ::write(m_interruptPipe[1], &byte, 1);
}

}
