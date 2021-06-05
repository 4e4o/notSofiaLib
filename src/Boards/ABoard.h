#ifndef A_BOARD_H
#define A_BOARD_H

#include <vector>

#include "Utils/Configurator/Configurator.h"

namespace boards {

// Абстрактная борда

class ABoard : public Configurator {
public:
    ~ABoard() {}
};

}

#endif // A_BOARD_H
