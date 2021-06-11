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

Subsystem::Subsystem(MPP* p)
    : MPPChild(p),
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
    parent()->registerType([](Subsystem* p, int id) -> Group* {
        return new Group(p, id);
    }, false);
    parent()->registerType([](Group* g, int id) -> Channel* {
        return new Channel(g, id);
    }, false);
}

VBPool* Subsystem::pool() const {
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
    for (auto& group : m_groups) {
        for (auto& channel : group->channels()) {
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

    for (auto& vpss_group : parent()->vpss()->groups()) {
        Group* group = addGroup(id);

        Channel* channel = group->addChannel(id++);
        channel->setSource(vpss_group);
        H264AttributesBuilder* ab = new H264AttributesBuilder();
        channel->setAttributesBuilder(ab);
        channel->setStreamOut(new StreamFileOut(channel));
//        channel->setStreamOut(new StreamDummyOut());

        bind(vpss_group, group);
    }
}

// Назначает лупы каналам по порядку
StreamLoop* Subsystem::getLoopForChannel() {
    if(m_streamLoops.empty())
        throw std::runtime_error("No stream loops");

    if (m_channelLoopIndex >= (int) m_streamLoops.size())
        m_channelLoopIndex = 0;

    return m_streamLoops[m_channelLoopIndex++];
}

Group* Subsystem::addGroup(int id) {
    Group* group = parent()->create<Group>(this, id);
    addItemBack(group);
    m_groups.push_back(group);
    return group;
}

int Subsystem::channelsCount() const {
    int count = 0;

    for (auto& group : m_groups)
        count += group->channels().size();

    return count;
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

const std::vector<Group*>& Subsystem::groups() const {
    return m_groups;
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
