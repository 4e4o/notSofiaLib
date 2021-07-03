#include "PipeInterrupter.h"

#include <unistd.h>
#include <stdexcept>
#include <fcntl.h>

static bool setNonBlocking(int fd) {
    return ::fcntl(fd, F_SETFL, O_NONBLOCK) != -1;
}

PipeInterrupter::PipeInterrupter(EpollLoop *l) {
    open();
    attachToLoop(l, EpoolOperationType{.READ = 1}, m_fds[0]);
}

PipeInterrupter::~PipeInterrupter() {
    removeFromLoop(m_fds[0]);
    close();
}

void PipeInterrupter::interrupt() {
    char byte = 1;
    ::write(m_fds[1], &byte, 1);
}

void PipeInterrupter::onRead() {

    for (;;) {
        char data;
        ssize_t bytes_read = ::read(m_fds[0], &data, sizeof(data));

        if ((bytes_read < 0) && (errno == EINTR))
            continue;

        if (bytes_read > 0)
            continue;

        return;
    }
}

void PipeInterrupter::open() {
    if (::pipe(m_fds) != 0)
        throw std::runtime_error("Create pipe failed");

    setNonBlocking(m_fds[0]);
    setNonBlocking(m_fds[1]);
}

void PipeInterrupter::close() {
    ::close(m_fds[0]);
    ::close(m_fds[1]);
}
