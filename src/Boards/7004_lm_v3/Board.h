#ifndef LM_7003_V3_BOARD_H
#define LM_7003_V3_BOARD_H

#include "Boards/nvp6134/Board.h"

namespace boards::lm7004v3 {

// lm7004v3 борда

class Board : public boards::nvp6134::Board {
  public:
    Board();
    ~Board();
};

}

#endif // LM_7003_V3_BOARD_H
