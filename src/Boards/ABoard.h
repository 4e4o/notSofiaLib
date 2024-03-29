#ifndef A_BOARD_H
#define A_BOARD_H

#include "Misc/Configurator/Configurator.h"
#include "Misc/Configurator/ConfigurableFactory.h"

namespace hisilicon::mpp {
class MPP;
}

namespace boards {

// Абстрактная плата с mpp

class ABoard : public Configurator, public ConfigurableFactory {
  public:
    ABoard();
    ~ABoard();

    void run();
    void stop();

    hisilicon::mpp::MPP *mpp() const;

  protected:
    bool configureImpl() override;

  private:
    hisilicon::mpp::MPP *m_mpp;
    bool m_stopped;
};

}

#endif // A_BOARD_H
