#include "AApplication.h"

#include <signal.h>

AApplication *AApplication::g_app = nullptr;

using boards::ABoard;

AApplication::AApplication() {
    g_app = this;
    setExit();
}

AApplication::~AApplication() {
}

AApplication *AApplication::app() {
    return g_app;
}

void AApplication::setExit() {
    signal(SIGINT, [](int) {
        signal(SIGINT, SIG_IGN);
        g_app->stop();
    });
}

int AApplication::run() {
    m_board.reset(create<ABoard>());

    if (!m_board->configure())
        throw std::runtime_error("board configure failed");

    if (!m_board->start())
        throw std::runtime_error("board start failed");

    m_board->run();
    return 0;
}

void AApplication::stop() {
    m_board->stop();
}
