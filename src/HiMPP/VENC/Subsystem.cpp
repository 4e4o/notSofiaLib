#include "Subsystem.h"
#include "HiMPP/MPP.h"
#include "Group.h"
#include "Channel/Channel.h"
#include "Channel/H264AttributesBuilder.h"
#include "HiMPP/VPSS/Subsystem.h"
#include "HiMPP/VPSS/Group.h"
#include "HiMPP/VPSS/Channel.h"
#include "HiMPP/VB/VBPool.h"
#include "Channel/StreamLoop.h"
#include "Channel/StreamFileOut.h"
#include "Channel/StreamDummyOut.h"
#include "Misc/Utils.h"

#define DEFAULT_STREAM_LOOPS_COUNT 1

namespace hisilicon::mpp::venc {

Subsystem::Subsystem(MPP *p)
    : ASubsystem(p),
      m_poolMode(PoolAllocationMode::PRIVATE_VB_POOL),
      m_pool(nullptr),
      m_streamLoopsCount(DEFAULT_STREAM_LOOPS_COUNT),
      m_channelLoopIndex(0) {
    registerDefaultTypes();
}

Subsystem::~Subsystem() {
    stop();
    joinStreamThreads();
    Utils::clearPtrContainer(m_threads);
    Utils::clearPtrContainer(m_streamLoops);
}

void Subsystem::registerDefaultTypes() {
    factory()->registerType([](Subsystem * p, int id) -> Group* {
        return new Group(p, id);
    }, false);
    factory()->registerType([](Group * g, int id) -> Channel* {
        return new Channel(g, id);
    }, false);
}

Group *Subsystem::addGroup(int id) {
    return addSubItem(this, id);
}

const std::vector<Group *> &Subsystem::groups() const {
    return subItems();
}

VBPool *Subsystem::pool() const {
    return m_pool;
}

bool Subsystem::configureImpl() {
    createUserPool();
    createStreamLoops();
    return ConfiguratorBinder::configureImpl();
}

void Subsystem::createUserPool() {
    if (!needUserPool())
        return;

    m_pool = new VBPool(parent());
    m_pool->initForVenc();
    addItemFront(m_pool);
}

bool Subsystem::needUserPool() {
    for (auto &group : groups()) {
        for (auto &channel : group->channels()) {
            if (channel->needUserPool())
                return true;
        }
    }

    return false;
}

// Добавляет venc группу на каждую группу vpss.
// в каждой группе по одному каналу.
// Должен быть вызван когда группы и каналы vpss уже добавлены.
void Subsystem::addSourceFromVpss1by1() {
    int id = 0;

    for (auto &vpss_group : parent()->vpss()->groups()) {
        Group *venc_group = addGroup(id);
        bind(vpss_group, venc_group);

        Channel *channel = venc_group->addChannel(id++);
        H264AttributesBuilder *ab = new H264AttributesBuilder();
        channel->setAttributesBuilder(ab);
        channel->setStreamOut(new StreamFileOut(channel));
        //        channel->setStreamOut(new StreamDummyOut());

    }
}

// Назначает лупы каналам по порядку
StreamLoop *Subsystem::getLoopForChannel() {
    if (m_streamLoops.empty())
        throw std::runtime_error("No stream loops");

    if (m_channelLoopIndex >= (int) m_streamLoops.size())
        m_channelLoopIndex = 0;

    return m_streamLoops[m_channelLoopIndex++];
}

void Subsystem::createStreamLoops() {
    for (int i = 0 ; i < m_streamLoopsCount ; i++)
        m_streamLoops.push_back(new StreamLoop());
}

void Subsystem::stop() {
    for (int i = 0 ; i < (int) m_streamLoops.size() ; i++)
        m_streamLoops[i]->stop();
}

void Subsystem::run() {
    if (m_streamLoops.empty())
        throw std::runtime_error("No stream loops to run");

    // создаём потоки для лупов > 0
    for (int i = 1 ; i < (int) m_streamLoops.size() ; i++) {
        m_threads.push_back(new std::thread([this, i] () {
            m_streamLoops[i]->run();
        }));
    }

    // первый луп будет в текущем потоке бегать
    m_streamLoops[0]->run();

    joinStreamThreads();
}

void Subsystem::joinStreamThreads() {
    for (int i = 0 ; i < (int) m_threads.size() ; i++)
        m_threads[i]->join();
}

void Subsystem::setStreamLoopsCount(int streamLoopsCount) {
    m_streamLoopsCount = streamLoopsCount;
}

void Subsystem::setPoolAllocationMode(PoolAllocationMode m) {
    // режим работы пула
    // если h264e.ko загружен с H264eVBSource == 1
    // то должен быть равен PRIVATE_VB_POOL
    // если загружен с H264eVBSource == 2 параметром
    // то == USER_VB_POOL
    // по дефолту == PRIVATE_VB_POOL
    // HiMPP Media Processing Software Development Reference.pdf
    // стр. 511
    m_poolMode = m;
}

Subsystem::PoolAllocationMode Subsystem::poolAllocationMode() const {
    return m_poolMode;
}

}
