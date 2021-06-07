#include "Board.h"

#include "HiMPP/Configurations/7004_lm_v3/MPP.h"
#include "ADC/nvp6134/DriverCommunicator.h"
#include "ADC/nvp6134/Configurations/7004_lm_v3/Chip.h"
#include "HiMPP/Configurations/hi3520dv200/VI/Channel.h"
#include "HiMPP/Configurations/hi3520dv200/VI/Device.h"
#include "HiMPP/Configurations/hi3520dv200/VI/Subsystem.h"
#include "Boards/nvp6134/HiMPP/ViInfoProvider.h"

#define NVP_CHIPS_COUNT 1

namespace boards::lm7004v3 {

using hisilicon::mpp::lm7004v3::MPP;

Board::Board()
    : boards::nvp6134::Board(NVP_CHIPS_COUNT) {
}

Board::~Board() {
}

::nvp6134::Chip* Board::createNvpChip(::nvp6134::DriverCommunicator* d, int i) {
    return new ::nvp6134::lm7004v3::Chip(d, i);
}

void Board::initialize() {
    using namespace hisilicon::mpp;

    MPP* mpp = new MPP();

    mpp->registerType([this]() -> vi::InfoProvider* {
        return new nvp6134::mpp::vi::InfoProvider(this);
    });

    mpp->registerType([](hisilicon::mpp::MPP* p) -> vi::Subsystem* {
        return new hisilicon::mpp::vi::hi3520dv200::Subsystem(p);
    });

    mpp->registerType([](vi::Subsystem* s, int id) -> vi::Device* {
        return new hisilicon::mpp::vi::hi3520dv200::Device(s, id);
    });

    mpp->registerType([](vi::Device* d, vi::ChannelInfo* i, int id) -> vi::Channel* {
        return new hisilicon::mpp::vi::hi3520dv200::Channel(d, i, id);
    });

    addItem(mpp);
}

// TODO remove it
hisilicon::mpp::MPP* Board::mpp() const {
    return static_cast<hisilicon::mpp::MPP*>(item(1));
}

}
