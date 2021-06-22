#include "ABoard.h"
#include "HiMPP/MPP.h"

namespace boards {

using ::hisilicon::mpp::MPP;

ABoard::ABoard()
    : m_mpp(nullptr),
      m_stopped(false) {
}

ABoard::~ABoard() {
}

bool ABoard::configureImpl() {
    addItemBack(m_mpp = create<MPP>());
    return Configurator::configureImpl();
}

hisilicon::mpp::MPP *ABoard::mpp() const {
    return m_mpp;
}

void ABoard::run() {
    if ((m_mpp == nullptr) || (m_stopped))
        return;

    m_mpp->run();
}

void ABoard::stop() {
    if (m_stopped)
        return;

    m_stopped = true;

    if (m_mpp == nullptr)
        return;

    m_mpp->stop();
}

}
