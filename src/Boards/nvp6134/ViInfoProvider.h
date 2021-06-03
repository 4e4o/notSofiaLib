#ifndef BOARD_NVP6134_VI_INFO_PROVIDER_H
#define BOARD_NVP6134_VI_INFO_PROVIDER_H

#include "Hisilicon/MPP/Vi/Source/ViInfoProvider.h"

#include <memory>

namespace boards {
namespace nvp6134 {

class BoardWithNvp6134;

class ViInfoNvp6134Provider : public hisilicon::mpp::ViInfoProvider {
public:
    ViInfoNvp6134Provider(BoardWithNvp6134*);

private:
    static TViDevicesInfo createViInfo(BoardWithNvp6134*);
};

}
}

#endif // BOARD_NVP6134_VI_INFO_PROVIDER_H
