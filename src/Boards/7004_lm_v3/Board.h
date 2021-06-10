#ifndef LM_7003_V3_BOARD_H
#define LM_7003_V3_BOARD_H

#include <memory>

#include "Boards/nvp6134/Board.h"

namespace nvp6134 {
class Chip;
class DriverCommunicator;
}

namespace hisilicon::mpp {
class MPP;
}

namespace boards::lm7004v3 {

// lm7004v3 борда

class Board : public boards::nvp6134::Board {
public:
    Board();
    ~Board();

    // TODO move it to board with mpp class
    void run();
    void stop();

private:
    void initialize() override final;
    ::nvp6134::Chip* createNvpChip(::nvp6134::DriverCommunicator*, int i) override final;

    hisilicon::mpp::MPP *m_mpp;
};

}

#endif // LM_7003_V3_BOARD_H
