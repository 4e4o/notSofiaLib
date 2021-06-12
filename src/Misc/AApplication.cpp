#include "AApplication.h"

#include <signal.h>
#include <iostream>

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

boards::ABoard *AApplication::board() const {
    return m_board.get();
}

int AApplication::run() {
    m_board.reset(create<ABoard>());

    try {
        if (!m_board->configure())
            throw std::runtime_error("board configure failed");

        beforeBoardRun();
        m_board->run();
    } catch (const std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown exception: " << std::endl;
    }

    return 0;
}

void AApplication::beforeBoardRun() {
}

void AApplication::stop() {
    m_board->stop();
}
