#include "AApplication.h"

#include <signal.h>
#include <iostream>
#include <string.h>

#define CATCH_EXCEPTIONS

AApplication *AApplication::g_app = nullptr;

using boards::ABoard;

AApplication::AApplication(int count, char **argv)
    : m_stopped(false) {
    setExit();

    for (int i = 0 ; i < count ; i++)
        m_args.emplace_back(argv[i], strlen(argv[i]));

    g_app = this;
}

AApplication::~AApplication() {
    signal(SIGINT, SIG_IGN);
}

AApplication *AApplication::app() {
    return g_app;
}

void AApplication::setExit() {
    signal(SIGINT, [](int) {
        if (g_app != nullptr)
            g_app->stop();
    });
}

const AApplication::TArgs &AApplication::args() const {
    return m_args;
}

boards::ABoard *AApplication::board() const {
    return m_board.get();
}

int AApplication::run() {
    if (m_stopped)
        return 0;

    m_board.reset(create<ABoard>());

#ifdef CATCH_EXCEPTIONS
    try {
#endif
        if (!m_board->configure())
            throw std::runtime_error("board configure failed");

        beforeBoardRun();
        m_board->run();
#ifdef CATCH_EXCEPTIONS
    } catch (const std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown exception: " << std::endl;
    }
#endif

    return 0;
}

void AApplication::beforeBoardRun() {
}

void AApplication::stop() {
    if (m_stopped)
        return;

    m_stopped = true;

    if (m_board.get() != nullptr)
        m_board->stop();
}
