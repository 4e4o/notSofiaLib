#include "Board.h"

#include "ADC/nvp6134/DriverCommunicator.h"
#include "ADC/nvp6134/Configurations/7004_lm_v3/Chip.h"
#include "ADC/nvp6134/ViChannel.h"
#include "ADC/nvp6134/Motion.h"

#include "HiMPP/VI/InfoProvider.h"

#include "HiMPP/Configurations/hi3520dv200/MPP.h"
#include "HiMPP/Configurations/hi3520dv200/VI/Subsystem.h"

#include "HiMPP/VI/Subsystem.h"
#include "HiMPP/VI/Device.h"
#include "HiMPP/VI/Channel.h"
#include "HiMPP/VI/ChannelAttributes.h"

#include "HiMPP/VPSS/Group.h"
#include "HiMPP/VPSS/Subsystem.h"
#include "HiMPP/VPSS/GroupAttributes.h"
#include "HiMPP/VPSS/ChannelAttributes.h"

#include "HiMPP/VENC/Subsystem.h"
#include "HiMPP/VENC/Group.h"
#include "HiMPP/VENC/Channel/H264Attributes.h"
#include "HiMPP/VENC/Channel/StreamReader.h"

#include "HiMPP/ASubsystem/ReadLoop/ReaderMemFileOut.h"
#include "HiMPP/ASubsystem/ReadLoop/ReaderDummyOut.h"

#include <iostream>

#define NVP_CHIPS_COUNT 1

namespace boards::lm7004v3 {

using hisilicon::mpp::hi3520dv200::MPP;

Board::Board()
    : boards::nvp6134::Board(NVP_CHIPS_COUNT) {
    registerType<::nvp6134::Chip, ::nvp6134::lm7004v3::Chip, ::nvp6134::DriverCommunicator *, int>();

    registerType([this]() -> ::hisilicon::mpp::MPP* {
        MPP *mpp = new MPP();

        mpp->registerType([this]() -> hisilicon::mpp::vi::InfoProvider* {
            return new mpp::vi::InfoProvider(this);
        });

        mpp->registerType([this](hisilicon::mpp::MPP * p) -> hisilicon::mpp::vi::Subsystem* {
            return initVi(p);
        });

        mpp->registerType([this](hisilicon::mpp::MPP * p) -> hisilicon::mpp::vpss::Subsystem* {
            return initVpss(p);
        });

        mpp->registerType([this](hisilicon::mpp::MPP * p) -> hisilicon::mpp::venc::Subsystem* {
            return initVenc(p);
        });

        mpp->addViSubsystem();
        mpp->addVpssSubsystem();
        mpp->addVencSubsystem();

        return mpp;
    });
}

Board::~Board() {
}

hisilicon::mpp::vi::Subsystem *Board::initVi(hisilicon::mpp::MPP *p) {
    using hisilicon::mpp::vi::hi3520dv200::Subsystem;
    Subsystem *s = new Subsystem(p);
    s->setHiMode(Subsystem::HI3520DV200_MODE::MODE_4CH_72OP);
    return s;
}

static void setVpssGroupAttributes(hisilicon::mpp::vpss::Group *g) {
    using Attr = hisilicon::mpp::vpss::GroupAttributes;
    g->attributes()->set<Attr::NoiseReduction>(true);
}

static void setVpssGroupParameters(hisilicon::mpp::vpss::Group *g) {
    using namespace hisilicon::mpp::vpss;
    using Params = GroupParameters;
    GroupParameters *p = new GroupParameters();

    p->set<Params::Contrast>(64);
    p->set<Params::ImageEnhancementStrength>(1);
    p->set<Params::SpatialNoiseReductionStrength>(4);
    p->set<Params::TimeNoiseReductionStrength>(8);

    if (g->id() == 0)
        p->set<Params::Crop>({0, 0, 718, 576});

    g->setAttributes(p);
}

hisilicon::mpp::vpss::Subsystem *Board::initVpss(hisilicon::mpp::MPP *p) {
    using namespace hisilicon::mpp::vpss;
    Subsystem *s = new Subsystem(p);

    // Добавляем vpss группу на каждый канал vi.
    // в каждой группе по одному каналу.
    {
        int groupId = 0;

        for (auto &device : p->vi()->devices()) {
            for (auto &channel : device->channels()) {
                //using Attr = hisilicon::mpp::vi::ChannelAttributes;
                //auto attr = channel->attributes();
                //attr->set<Attr::FrameRate>(10);

                Group *group = s->addGroup(groupId++);
                setVpssGroupAttributes(group);
                setVpssGroupParameters(group);
                // id vpss каналов фиксированы и зависят от хики чипа
                // определённый канал с id обладает определёнными свойствами и функциями
                // мы тут юзаем Hi3520d 0 (VPSS_BSTR_CHN) vpss канал
                // он юзается для "Large stream encoding"
                // HiMPP Media Processing Software Development Reference.pdf
                // page 422
                group->addChannel(VPSS_BSTR_CHN);
                s->bind(channel, group);
            }
        }
    }

    return s;
}

hisilicon::mpp::venc::Subsystem *Board::initVenc(hisilicon::mpp::MPP *p) {
    using namespace hisilicon::mpp::venc;
    Subsystem *s = new Subsystem(p);
    // это зависит от того с какими параметрами загружен h264e.ko
    // см. комментарии внутри setPoolAllocationMode
    s->setPoolAllocationMode(Subsystem::PoolAllocationMode::USER_VB_POOL);

    // Добавляем venc группу на каждую группу vpss.
    // в каждой группе по одному каналу.
    {
        int id = 0;

        for (auto &vpss_group : p->vpss()->groups()) {
            Group *venc_group = s->addGroup(id);
            s->bind(vpss_group, venc_group);

            Channel *channel = venc_group->addChannel(id++);
            H264Attributes *ab = new H264Attributes();

            ab->set<H264Attributes::ProfileType>(H264Attributes::TProfile::HIGH);

            if (channel->id() == 0)
                ab->set<H264Attributes::Bpp>(0.04f);
            else
                ab->set<H264Attributes::Bpp>(0.02f);

            channel->setAttributes<ChannelAttributes>(ab);
            initVencChannel(channel);
        }
    }

    return s;
}

void Board::initVencChannel(hisilicon::mpp::venc::Channel *c) {
    setStreamOut(c);
    setMotion(c);
}

void Board::setStreamOut(hisilicon::mpp::venc::Channel *c) {
    using namespace hisilicon::mpp::venc;

    using Out = hisilicon::mpp::ReaderMemFileOut;
    const std::string fname = "stream_" + std::to_string(c->id()) + ".h264";
    const size_t buf_size = 512 * 1024;
    c->streamReader()->setOut(new Out(fname, buf_size), true);

    //using Out = hisilicon::mpp::ReaderFileOut;
    //c->streamReader()->setOut(new Out(fname), true);

    //using Out = hisilicon::mpp::ReaderDummyOut;
    //c->streamReader()->setOut(new Out(), true);
}

void Board::setMotion(hisilicon::mpp::venc::Channel *c) {
    ::nvp6134::ViChannel *ch = viChannel(c);
    ::nvp6134::Motion *motion = ch->motion();

    using namespace ::nvp6134;
    setMotionEvent(c, motion);
    motion->setAreaAll(true);
    motion->setTemporalSensitivity(Motion::TemporalSensitivity::HIGH);
    //motion->setVisualize(true);
    motion->setEnabled(true);
}

void Board::setMotionEvent(hisilicon::mpp::venc::Channel *,
                           ::nvp6134::Motion *motion) {
    using namespace ::nvp6134;
    motion->setEvent([](Motion * m) {
        std::cout << "MOTION EVENT!!! " << m->channel()->id() << std::endl;
    });
}

}
