#include <signal.h>

#include <stdexcept>
#include <iostream>

#include "Boards/7004_lm_v3/Board.h"

static boards::lm7004v3::Board *board;

int main() {

    board = new boards::lm7004v3::Board();

    if (!board->configure())
        throw std::runtime_error("board configure failed");

    if (!board->start())
        throw std::runtime_error("board start failed");

    signal(SIGINT, [](int) {
        signal(SIGINT, SIG_IGN);
        board->stop();
    });

    board->run();

    delete board;

    return 0;
}
