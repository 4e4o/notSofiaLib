#ifndef LM_7003_V3_BOARD_H
#define LM_7003_V3_BOARD_H

#include "Boards/nvp6134/Board.h"

namespace hisilicon::mpp {
class MPP;
namespace vi {
class Subsystem;
}
namespace vpss {
class Subsystem;
}
namespace venc {
class Subsystem;
class Channel;
}
}

namespace boards::lm7004v3 {

// lm7004v3 борда

class Board : public boards::nvp6134::Board {
  public:
    Board();
    ~Board();

  protected:
    bool configureImpl() override;

  private:
    virtual void setStreamOut(hisilicon::mpp::venc::Channel *);
    void setNvpMotion();

    hisilicon::mpp::vi::Subsystem *initVi(hisilicon::mpp::MPP *p);
    hisilicon::mpp::vpss::Subsystem *initVpss(hisilicon::mpp::MPP *p);
    hisilicon::mpp::venc::Subsystem *initVenc(hisilicon::mpp::MPP *p);
};

}

#endif // LM_7003_V3_BOARD_H
