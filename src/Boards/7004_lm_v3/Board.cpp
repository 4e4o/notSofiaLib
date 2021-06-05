#include "Board.h"

#include "ADC/nvp6134/Chip.h"
#include "ADC/nvp6134/DriverCommunicator.h"
#include "nvp6134/Chip.h"
#include "HiMPP/MPP.h"
#include "Boards/nvp6134/MPP/ElementsFactory.h"
#include "HiMPP/Configurations/hi3520dv200/ElementsFactory.h"

#define NVP_CHIPS_COUNT 1

namespace boards {
namespace lm7004v3 {

using hisilicon::mpp::MPP;
using boards::nvp6134::mpp::ElementsFactory;
typedef ElementsFactory<hisilicon::mpp::hi3520dv200::ElementsFactory> TElemFactory;

Board::Board()
    : boards::nvp6134::Board(NVP_CHIPS_COUNT) {
}

Board::~Board() {
}

::nvp6134::Chip* Board::createNvpChip(::nvp6134::DriverCommunicator* d, int i) {
    return new nvp6134::Chip(d, i);
}

void Board::initialize() {
    MPP* mpp = new MPP(new TElemFactory(this));
    mpp->addViSubsystem();
    addItem(mpp);
}

// TODO remove it
hisilicon::mpp::MPP* Board::mpp() const {
    return static_cast<hisilicon::mpp::MPP*>(item(1));
}

}
}
