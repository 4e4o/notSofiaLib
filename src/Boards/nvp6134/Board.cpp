#include "Board.h"

#include "HiMPP/VI/ChannelInfo.h"

#include "ADC/nvp6134/DriverCommunicator.h"
#include "ADC/nvp6134/Chip.h"
#include "ADC/nvp6134/ViChannel.h"

#include "HiMPP/VENC/Channel/Channel.h"
#include "HiMPP/VENC/Group.h"
#include "HiMPP/VI/Channel.h"

namespace boards::nvp6134 {

Board::Board(int chipCount) :
    m_chipCount(chipCount),
    m_nvpDriver(nullptr) {
    registerDefaultType<::nvp6134::DriverCommunicator, int>();
    registerDefaultType<::nvp6134::Chip, ::nvp6134::DriverCommunicator *, int>();
}

bool Board::configureImpl() {
    m_nvpDriver.reset(create<::nvp6134::DriverCommunicator>(m_chipCount));
    m_nvpDriver->configure();

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

::nvp6134::ViChannel *Board::nvpViChannel(hisilicon::mpp::venc::Channel *c)
const {
    using namespace ::hisilicon::mpp;
    using vi::Channel;
    using vpss::BindItem;
    const Channel *viChannel = Channel::associatedChannel(c, c->group());

    if (viChannel == nullptr)
        return nullptr;

    using boards::nvp6134::mpp::vi::ChannelInfo;
    const ChannelInfo *ci = dynamic_cast<const ChannelInfo *>(viChannel->source());

    if (ci == nullptr)
        return nullptr;

    return ci->viChannel();
}

}
