#include "Board.h"

#include "HiMPP/Configurations/7004_lm_v3/MPP.h"
#include "ADC/nvp6134/DriverCommunicator.h"
#include "ADC/nvp6134/Configurations/7004_lm_v3/Chip.h"
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

    addItem(mpp);
}

// TODO remove it
hisilicon::mpp::MPP* Board::mpp() const {
    return static_cast<hisilicon::mpp::MPP*>(item(1));
}

}
