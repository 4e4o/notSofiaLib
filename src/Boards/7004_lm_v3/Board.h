#ifndef LM_7003_V3_BOARD_H
#define LM_7003_V3_BOARD_H

#include <memory>

#include "Boards/nvp6134/Board.h"

namespace nvp6134 {
class Chip;
class DriverCommunicator;
}

namespace hisilicon {
namespace mpp {
class MPP;
}
}

namespace boards {
namespace lm7004v3 {

class Board : public boards::nvp6134::Board {
public:
    Board();

    hisilicon::mpp::MPP* mpp() const;

private:
    void onChipsCreated();
    ::nvp6134::Chip* createNvpChip(::nvp6134::DriverCommunicator*, int i);
};

}
}

#endif // LM_7003_V3_BOARD_H
