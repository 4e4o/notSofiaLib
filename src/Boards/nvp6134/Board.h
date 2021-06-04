#ifndef BOARD_WITH_NVP6134_H
#define BOARD_WITH_NVP6134_H

#include <memory>

#include "Boards/ABoard.h"

namespace nvp6134 {
class Chip;
class DriverCommunicator;
}

namespace boards {
namespace nvp6134 {

class BoardWithNvp6134 : public ABoard {
public:
    BoardWithNvp6134(int chipCount);
    ~BoardWithNvp6134();

    ::nvp6134::Chip* nvp(int = 0) const;
    int nvpCount() const;

protected:
    virtual ::nvp6134::Chip* createNvpChip(::nvp6134::DriverCommunicator*, int i) = 0;
protected:
    bool configureImpl();

private:
    std::unique_ptr< ::nvp6134::DriverCommunicator> m_nvpDriver;
    const int m_nvpStartIndex;
    const int m_nvpCount;
};

}
}

#endif // LM_7003_V3_BOARD_H
