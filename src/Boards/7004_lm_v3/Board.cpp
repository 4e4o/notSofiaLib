#include "Board.h"

#include "ADC/nvp6134/DriverCommunicator.h"
#include "ADC/nvp6134/Configurations/7004_lm_v3/Chip.h"
#include "Boards/nvp6134/HiMPP/ViInfoProvider.h"

#include "HiMPP/Configurations/hi3520dv200/MPP.h"
#include "HiMPP/Configurations/hi3520dv200/VI/Subsystem.h"

#include "HiMPP/VI/Subsystem.h"
#include "HiMPP/VI/Device.h"
#include "HiMPP/VI/Channel.h"

#include "HiMPP/VPSS/Group.h"
#include "HiMPP/VPSS/Subsystem.h"

#include "HiMPP/VENC/Subsystem.h"
#include "HiMPP/VENC/Group.h"
#include "HiMPP/VENC/Channel/H264AttributesBuilder.h"
#include "HiMPP/VENC/Channel/StreamFileOut.h"
#include "HiMPP/VENC/Channel/StreamDummyOut.h"

#define NVP_CHIPS_COUNT 1

namespace boards::lm7004v3 {

using hisilicon::mpp::hi3520dv200::MPP;

Board::Board()
    : boards::nvp6134::Board(NVP_CHIPS_COUNT) {
    registerType<::nvp6134::Chip, ::nvp6134::lm7004v3::Chip, ::nvp6134::DriverCommunicator *, int>();

    registerType([this]() -> ::hisilicon::mpp::MPP* {
        MPP *mpp = new MPP();

        mpp->registerType([this]() -> hisilicon::mpp::vi::InfoProvider* {
            return new nvp6134::mpp::vi::InfoProvider(this);
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
    g->enableNoiseReduction();
}

static void setVpssGroupParameters(hisilicon::mpp::vpss::Group *g) {
    VPSS_GRP_PARAM_S *p = new VPSS_GRP_PARAM_S{};

    p->u32Contrast = 64;
    p->u32IeStrength = 1;
    p->u32SfStrength = 4;
    p->u32TfStrength = 8;
    // незнаю зачем это, в мане описания нету - написано Reserved
    // София также делает, пусть будет так же
    p->u32MotionThresh = 2;

    g->setParameters(p);
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
            H264AttributesBuilder *ab = new H264AttributesBuilder();
            channel->setAttributesBuilder(ab);
            setStreamOut(channel);
        }
    }

    return s;
}

void Board::setStreamOut(hisilicon::mpp::venc::Channel *c) {
    using namespace hisilicon::mpp::venc;
    c->setStreamOut(new StreamFileOut(c), true);
    //        c->setStreamOut(new StreamDummyOut());
}

}
