#ifndef BOARD_NVP6134_VI_INFO_PROVIDER_H
#define BOARD_NVP6134_VI_INFO_PROVIDER_H

#include "Hisilicon/MPP/VI/Source/InfoProvider.h"

#include <memory>

namespace boards {
namespace nvp6134 {

class BoardWithNvp6134;

class ViInfoProvider : public hisilicon::mpp::vi::InfoProvider {
public:
    ViInfoProvider(BoardWithNvp6134*);

private:
    static TViDevicesInfo createViInfo(BoardWithNvp6134*);
};

}
}

#endif // BOARD_NVP6134_VI_INFO_PROVIDER_H
