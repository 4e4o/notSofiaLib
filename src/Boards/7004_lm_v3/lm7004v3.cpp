#include "lm7004v3.h"

#include "ADC/nvp6134/Chip.h"
#include "ADC/nvp6134/DriverCommunicator.h"
#include "nvp6134/Chip.h"

#define NVP_CHIPS_COUNT 1

namespace boards {
namespace lm7004v3 {

//using namespace nvp6134;

Lm7004v3Board::Lm7004v3Board() :
    m_nvpDriver(new ::nvp6134::DriverCommunicator(NVP_CHIPS_COUNT)),
    m_nvpChip(new nvp6134::Chip(m_nvpDriver.get(), NVP_CHIPS_COUNT - 1)) {
}

bool Lm7004v3Board::configure() {
    m_nvpChip->configure();
    return true;
}

::nvp6134::Chip* Lm7004v3Board::nvp() const {
    return m_nvpChip.get();
}

}
}
