#include "Board.h"

#include "ADC/nvp6134/Chip.h"
#include "ADC/nvp6134/DriverCommunicator.h"

namespace boards::nvp6134 {

Board::Board(int chipCount) :
    m_nvpDriver(new ::nvp6134::DriverCommunicator(chipCount)),
    m_nvpStartIndex(itemsCount()),
    m_nvpCount(chipCount) {
}

bool Board::configureImpl() {
    for (int i = 0 ; i < m_nvpCount ; i++)
        addItem(createNvpChip(m_nvpDriver.get(), i));

    initialize();
    return ABoard::configureImpl();
}

void Board::initialize() {
}

int Board::nvpCount() const {
    return m_nvpCount;
}

::nvp6134::Chip* Board::nvp(int i) const {
    return static_cast< ::nvp6134::Chip*>(item(m_nvpStartIndex + i));
}

}
