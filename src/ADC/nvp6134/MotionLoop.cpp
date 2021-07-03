#include "MotionLoop.h"
#include "DriverCommunicator.h"
#include "Misc/EventLoop/Timer.h"

#include <iostream>

// читаем раз в две секунды, торопиться нам некуда
// потому что мы читаем с драйвера в hold mode.
// Ни один сдетекченный эвент от nvp от нас не уйдет
#define CHECK_INTERVAL_SEC 2

namespace nvp6134 {

MotionLoop::MotionLoop(DriverCommunicator *dc)
    : Holder<DriverCommunicator *>(dc),
      m_readTimer(new Timer(this)) {
    m_readTimer->setTick([this](Timer *) {
        this->onTick();
    });
    m_readTimer->start(std::chrono::seconds(CHECK_INTERVAL_SEC));
    std::cout << "MotionLoop " << this << std::endl;
}

MotionLoop::~MotionLoop() {
    std::cout << "~MotionLoop " << this << std::endl;
}

void MotionLoop::onTick() {
    TChannels curIds;

    {
        std::lock_guard lck(m_channelsMutex);

        for (const auto &i : m_channels)
            curIds.insert(i);
    }

    if (curIds.empty()) {
        std::cout << "MotionLoop::onTimeout nothing to process" << std::endl;
        return;
    }

    auto ids = value()->getMotion();

    for (auto const &i : ids) {
        if (curIds.contains(i))
            curIds[i]();
    }
}

void MotionLoop::watch(TChipChannel id, TMotionEvent ev) {
    std::lock_guard lck(m_channelsMutex);
    m_channels.erase(id);
    m_channels[id] = ev;
}

void MotionLoop::remove(TChipChannel id) {
    std::lock_guard lck(m_channelsMutex);
    m_channels.erase(id);
}

}
