#include "Board.h"

#include "ADC/nvp6134/DriverCommunicator.h"
#include "ADC/nvp6134/Configurations/7004_lm_v3/Chip.h"
#include "Boards/nvp6134/HiMPP/ViInfoProvider.h"
#include "HiMPP/Configurations/hi3520dv200/MPP.h"
#include "HiMPP/Configurations/hi3520dv200/VI/Subsystem.h"
#include "HiMPP/VPSS/Subsystem.h"
#include "HiMPP/VENC/Subsystem.h"

#define NVP_CHIPS_COUNT 1

namespace boards::lm7004v3 {

using hisilicon::mpp::hi3520dv200::MPP;

Board::Board()
    : boards::nvp6134::Board(NVP_CHIPS_COUNT) {
    registerType([](::nvp6134::DriverCommunicator * d, int id) -> ::nvp6134::Chip* {
        return new ::nvp6134::lm7004v3::Chip(d, id);
    });

    registerType([this]() -> ::hisilicon::mpp::MPP* {
        MPP *mpp = new MPP();

        mpp->registerType([this]() -> hisilicon::mpp::vi::InfoProvider* {
            return new nvp6134::mpp::vi::InfoProvider(this);
        });

        mpp->registerType([](hisilicon::mpp::MPP * p) -> hisilicon::mpp::vi::Subsystem* {
            using hisilicon::mpp::vi::hi3520dv200::Subsystem;
            Subsystem *s = new Subsystem(p);
            s->setHiMode(Subsystem::HI3520DV200_MODE::MODE_4CH_72OP);
            return s;
        });

        mpp->registerType([](hisilicon::mpp::MPP * p) ->
        hisilicon::mpp::vpss::Subsystem* {
            using hisilicon::mpp::vpss::Subsystem;
            Subsystem *s = new Subsystem(p);
            s->addSourceFromVi1by1();
            return s;
        });

        mpp->registerType([](hisilicon::mpp::MPP * p) ->
        hisilicon::mpp::venc::Subsystem* {
            using hisilicon::mpp::venc::Subsystem;
            Subsystem *s = new Subsystem(p);
            // это зависит от того с какими параметрами загружен h264e.ko
            // см. комментарии внутри setPoolAllocationMode
            s->setPoolAllocationMode(Subsystem::PoolAllocationMode::USER_VB_POOL);
            s->addSourceFromVpss1by1();
            return s;
        });

        mpp->addViSubsystem();
        mpp->addVpssSubsystem();
        mpp->addVencSubsystem();

        return mpp;
    });
}

Board::~Board() {
}

}
