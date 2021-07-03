#include "EventLoop.h"
#include "PipeInterrupter.h"

EventLoop::EventLoop()
    : m_interrupter(new PipeInterrupter(this)) {
}

EventLoop::~EventLoop() {
}

void EventLoop::onPost() {
    m_interrupter->interrupt();
}

void EventLoop::iteration() {
    EventQueue::process();
}

void EventLoop::stop() {
    EpollLoop::stop();
    m_interrupter->interrupt();
}
