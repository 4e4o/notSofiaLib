#include "Epollable.h"
#include "EpollLoop.h"

Epollable::Epollable()
    : m_loop(nullptr) {
}

Epollable::~Epollable() {
}

void Epollable::onRead() {
}

void Epollable::onWrite() {
}

void Epollable::onError() {
}

EpollLoop *Epollable::loop() const {
    return m_loop;
}

void Epollable::attachToLoop(EpollLoop *l, EpoolOperationType op, int fd) {
    m_loop = l;
    l->add(op, this, fd);
}

void Epollable::removeFromLoop(int fd) {
    loop()->remove(this, fd);
}
