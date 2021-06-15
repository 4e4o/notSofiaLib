#include "Board.h"
#include "ADC/nvp6134/DriverCommunicator.h"
#include "ADC/nvp6134/Chip.h"

namespace boards::nvp6134 {

Board::Board(int chipCount) :
    m_chipCount(chipCount),
    m_nvpDriver(new ::nvp6134::DriverCommunicator(chipCount)) {
    registerDefaultType<::nvp6134::Chip, ::nvp6134::DriverCommunicator *, int>();
}

bool Board::configureImpl() {
    createChipsets();

    for (auto &chip : m_nvpChipsets)
        addItemBack(chip);

    // сразу конфигурируем чипы
    // чтобы mpp далее уже создавать с данными от
    // этих чипов
    for (auto &chip : m_nvpChipsets)
        chip->configure();

    return ABoard::configureImpl();
}

void Board::createChipsets() {
    for (int i = 0 ; i < m_chipCount ; i++)
        m_nvpChipsets.push_back(create<::nvp6134::Chip>(m_nvpDriver.get(), i));
}

const Board::TNvpChipsets &Board::nvp() const {
    return m_nvpChipsets;
}

}
