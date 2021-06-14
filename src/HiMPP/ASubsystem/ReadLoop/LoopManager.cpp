#include "LoopManager.h"
#include "ReadLoop.h"
#include "Misc/Utils.h"

#define DEFAULT_READ_LOOPS_COUNT 1

namespace hisilicon::mpp {

LoopManager::LoopManager()
    : m_readLoops(DEFAULT_READ_LOOPS_COUNT),
      m_assignLoopIndex(0) {
}

LoopManager::~LoopManager() {
    stop();
    join();
    Utils::clearPtrContainer(m_threads);
    Utils::clearPtrContainer(m_readLoops);
}

// Назначает лупы объектам по порядку
ReadLoop *LoopManager::assignLoop() {
    if (m_readLoops.empty())
        throw std::runtime_error("No read loops");

    if (m_assignLoopIndex >= (int) m_readLoops.size())
        m_assignLoopIndex = 0;

    return m_readLoops[m_assignLoopIndex++];
}

void LoopManager::createLoops() {
    for (auto &loop : m_readLoops)
        loop = new ReadLoop();
}

void LoopManager::stop() {
    for (auto &loop : m_readLoops)
        loop->stop();
}

void LoopManager::run() {
    if (m_readLoops.empty())
        throw std::runtime_error("No loops to run");

    // создаём потоки для лупов > 0
    for (int i = 1 ; i < (int) m_readLoops.size() ; i++) {
        m_threads.push_back(new std::thread([this, i] () {
            m_readLoops[i]->run();
        }));
    }

    // первый луп будет в текущем потоке бегать
    m_readLoops[0]->run();

    join();
}

void LoopManager::join() {
    for (int i = 0 ; i < (int) m_threads.size() ; i++)
        m_threads[i]->join();
}

void LoopManager::setLoopsCount(int loopsCount) {
    m_readLoops.resize(loopsCount);
}

}
