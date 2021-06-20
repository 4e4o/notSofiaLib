#ifndef LM_7003_V3_BOARD_VI_INFO_PROVIDER_H
#define LM_7003_V3_BOARD_VI_INFO_PROVIDER_H

#include "Boards/nvp6134/HiMPP/ViInfoProvider.h"

namespace boards::lm7004v3::mpp::vi {

class InfoProvider : public nvp6134::mpp::vi::InfoProvider {
  public:
    using nvp6134::mpp::vi::InfoProvider::InfoProvider;

  private:
    TChannelId inputIdToVi(const TChannelId &) override final;
};

}

#endif // LM_7003_V3_BOARD_VI_INFO_PROVIDER_H
