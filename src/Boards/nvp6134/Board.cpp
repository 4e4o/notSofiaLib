#include "Board.h"
#include "ADC/nvp6134/DriverCommunicator.h"
#include "ADC/nvp6134/Chip.h"

namespace boards::nvp6134 {

Board::Board(int chipCount) :
    m_chipCount(chipCount),
    m_nvpDriver(new ::nvp6134::DriverCommunicator(chipCount)) {
    registerType([](::nvp6134::DriverCommunicator* d, int id) -> ::nvp6134::Chip* {
        return new ::nvp6134::Chip(d, id);
    }, false);
}

bool Board::configureImpl() {
    createChipsets();

    // добавляем с конца в начало
    // потому что чипы должны первыми конфигурироваться
    for(auto it = m_nvpChipsets.rbegin() ; it != m_nvpChipsets.rend() ; it++)
        addItemFront(*it);

    return ABoard::configureImpl();
}

void Board::createChipsets() {
    for (int i = 0 ; i < m_chipCount ; i++)
        m_nvpChipsets.push_back(create<::nvp6134::Chip>(m_nvpDriver.get(), i));
}

const Board::TNvpChipsets& Board::nvp() const {
    return m_nvpChipsets;
}

}
