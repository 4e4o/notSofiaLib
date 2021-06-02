#ifndef LM_7003_V3_BOARD_H
#define LM_7003_V3_BOARD_H

#include <memory>

#include "Boards/ABoard.h"

namespace nvp6134 {
class Chip;
class DriverCommunicator;
}

namespace boards {
namespace lm7004v3 {

class Lm7004v3Board : public ABoard {
public:
    Lm7004v3Board();

    bool configure();
    ::nvp6134::Chip* nvp() const;

private:

    std::unique_ptr<nvp6134::DriverCommunicator> m_nvpDriver;
    std::unique_ptr<nvp6134::Chip> m_nvpChip;
};

}
}

#endif // LM_7003_V3_BOARD_H
