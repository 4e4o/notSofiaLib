#include "MotionLoop.h"
#include "DriverCommunicator.h"

#include <iostream>

#define DEFAULT_MOTION_CHECK_INTERVAL_MILLIS 1000

namespace nvp6134 {

MotionLoop::MotionLoop(DriverCommunicator *dc)
    : Holder<DriverCommunicator *>(dc) {
    setTimeout(std::chrono::milliseconds(DEFAULT_MOTION_CHECK_INTERVAL_MILLIS));
    std::cout << "MotionLoop " << this << std::endl;
}

MotionLoop::~MotionLoop() {
    std::cout << "~MotionLoop " << this << std::endl;
}

void MotionLoop::onTimeout() {
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
