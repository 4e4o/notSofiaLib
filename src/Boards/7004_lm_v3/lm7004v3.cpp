#include "lm7004v3.h"

#include "ADC/nvp6134/Chip.h"
#include "ADC/nvp6134/DriverCommunicator.h"
#include "nvp6134/Chip.h"
#include "Hisilicon/Chip/hi3520dv200/MPP/MPP.h"
#include "Boards/nvp6134/ViInfoProvider.h"

#define NVP_CHIPS_COUNT 1

namespace boards {
namespace lm7004v3 {

using hisilicon::mpp::hi3520dv200::MPP;
using boards::nvp6134::ViInfoProvider;

Lm7004v3Board::Lm7004v3Board()
    : boards::nvp6134::BoardWithNvp6134(NVP_CHIPS_COUNT) {
}

Lm7004v3Board::~Lm7004v3Board(){
}

bool Lm7004v3Board::configure() {
    BoardWithNvp6134::configure();

    m_mpp.reset(new MPP(new ViInfoProvider(this)));
    m_mpp->configure();

    return true;
}

::nvp6134::Chip* Lm7004v3Board::createNvpChip(::nvp6134::DriverCommunicator* d, int i) {
    return new nvp6134::Chip(d, i);
}

hisilicon::mpp::MPP* Lm7004v3Board::mpp() const {
    return m_mpp.get();
}

}
}
