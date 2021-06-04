#ifndef BOARD_NVP6134_VI_INFO_PROVIDER_H
#define BOARD_NVP6134_VI_INFO_PROVIDER_H

#include "Hisilicon/MPP/VI/Source/InfoProvider.h"
#include "Utils/Holder.h"

#include <memory>

namespace boards {
namespace nvp6134 {

class Board;

namespace mpp {
namespace vi {

class InfoProvider : public Holder<Board*>,
        public hisilicon::mpp::vi::InfoProvider {
public:
    InfoProvider(Board*);

private:
    bool configureImpl();
};

}
}
}
}

#endif // BOARD_NVP6134_VI_INFO_PROVIDER_H
