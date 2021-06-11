#include "ABoard.h"
#include "HiMPP/MPP.h"

namespace boards {

using ::hisilicon::mpp::MPP;

ABoard::ABoard()
    : m_mpp(nullptr) {
}

ABoard::~ABoard() {
}

bool ABoard::configureImpl() {
    addItemBack(m_mpp = create<MPP>());
    return Configurator::configureImpl();
}

void ABoard::run() {
    m_mpp->run();
}

void ABoard::stop() {
    m_mpp->stop();
}

}
