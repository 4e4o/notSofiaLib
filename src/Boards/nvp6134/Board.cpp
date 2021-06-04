#include "Board.h"

#include "ADC/nvp6134/Chip.h"
#include "ADC/nvp6134/DriverCommunicator.h"

namespace boards {
namespace nvp6134 {

BoardWithNvp6134::BoardWithNvp6134(int chipCount) :
    m_nvpDriver(new ::nvp6134::DriverCommunicator(chipCount)),
    m_nvpStartIndex(itemsCount()),
    m_nvpCount(chipCount) {
}

BoardWithNvp6134::~BoardWithNvp6134() {
}

bool BoardWithNvp6134::configureImpl() {
    for (int i = 0 ; i < m_nvpCount ; i++)
        addItem(createNvpChip(m_nvpDriver.get(), i));

    return ABoard::configureImpl();
}

int BoardWithNvp6134::nvpCount() const {
    return m_nvpCount;
}

::nvp6134::Chip* BoardWithNvp6134::nvp(int i) const {
    return static_cast< ::nvp6134::Chip*>(item(m_nvpStartIndex + i));
}

}
}
