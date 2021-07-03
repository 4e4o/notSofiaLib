#include "EpollLoop.h"
#include "Epollable.h"

#include <sys/epoll.h>
#include <vector>

#include <iostream>

#define EPOLL_SIZE      1024
#define WAIT_INFINITE   -1

EpollLoop::EpollLoop()
    : m_fd(INVALID_FD),
      m_running(false) {
    std::cout << "EpollLoop::EpollLoop " << this << std::endl;
    createEpoll();
}

EpollLoop::~EpollLoop() {
    std::cout << "EpollLoop::~EpollLoop " << this << std::endl;
    if (m_fd != INVALID_FD)
        close(m_fd);
}

static bool isErrorEvent(const epoll_event &e) {
    return (e.events & EPOLLERR) || (e.events & EPOLLHUP);
}

void EpollLoop::run() {
    m_running = true;

    if (m_fd == INVALID_FD)
        return;

    std::vector<epoll_event> events(EPOLL_SIZE);

    while (m_running) {
        //        std::cout << "EpollLoop::run iter " << this << std::endl;
        iteration();

        const int n = epoll_wait(m_fd, events.data(), EPOLL_SIZE, WAIT_INFINITE);

        for (int i = 0; i < n; i++) {
            epoll_event &event = events[i];
            Epollable *e = reinterpret_cast<Epollable *>(event.data.ptr);

            if (event.events & EPOLLIN)
                e->onRead();

            if (event.events & EPOLLOUT)
                e->onWrite();

            if (isErrorEvent(event))
                e->onError();
        }
    }
}

void EpollLoop::iteration() {
}

bool EpollLoop::add(EpoolOperationType op, Epollable *e, int fd) {
    epoll_event event{};

    event.data.fd = fd;
    event.data.ptr = e;
    event.events = 0;

    if (fd == INVALID_FD)
        throw std::runtime_error("EpollLoop::add fd is invalid");

    if (op.READ)
        event.events |= EPOLLIN;

    if (op.WRITE)
        event.events |= EPOLLOUT;

    if (epoll_ctl(m_fd, EPOLL_CTL_ADD, fd, &event) == -1)
        throw std::runtime_error("EpollLoop::add epoll_ctl failed");

    return true;
}

bool EpollLoop::remove(Epollable *e, int fd) {
    epoll_event event;

    event.data.fd = fd;
    event.data.ptr = e;

    if (fd == INVALID_FD)
        return false;

    if (epoll_ctl(m_fd, EPOLL_CTL_DEL, fd, &event) == -1)
        throw std::runtime_error("EpollLoop::remove epoll_ctl failed");

    return true;
}

void EpollLoop::stop() {
    m_running = false;
}

bool EpollLoop::createEpoll() {
    m_fd = epoll_create(EPOLL_SIZE);

    if (m_fd == INVALID_FD)
        throw std::runtime_error("epoll_create1 failed");

    return true;
}
