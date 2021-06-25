#ifndef BOARD_WITH_NVP6134_H
#define BOARD_WITH_NVP6134_H

#include <memory>

#include "Boards/ABoard.h"

namespace hisilicon::mpp::venc {
class Channel;
}

namespace nvp6134 {
class Chip;
class DriverCommunicator;
class ViChannel;
}

namespace boards::nvp6134 {

// Плата с nvp чипами

class Board : public ABoard {
  public:
    typedef std::vector<::nvp6134::Chip *> TNvpChipsets;

    Board(int chipCount);

    const TNvpChipsets &nvp() const;

  protected:
    bool configureImpl() override;

    ::nvp6134::ViChannel *viChannel(hisilicon::mpp::venc::Channel *) const;

  private:
    void createChipsets();

    int m_chipCount;
    std::unique_ptr<::nvp6134::DriverCommunicator> m_nvpDriver;
    TNvpChipsets m_nvpChipsets;
};

}

#endif // LM_7003_V3_BOARD_H
