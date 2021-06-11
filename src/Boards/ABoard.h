#ifndef A_BOARD_H
#define A_BOARD_H

#include <vector>

#include "Misc/Configurator/Configurator.h"
#include "Misc/Configurator/ConfigurableFactory.h"

namespace boards {

// Абстрактная борда

class ABoard : public Configurator, public ConfigurableFactory {
  public:
    ~ABoard() {}
};

}

#endif // A_BOARD_H
