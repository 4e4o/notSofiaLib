#ifndef BOARD_NVP6134_VI_INFO_PROVIDER_H
#define BOARD_NVP6134_VI_INFO_PROVIDER_H

#include "HiMPP/VI/Source/InfoProvider.h"
#include "Utils/Holder.h"

#include <memory>

namespace boards::nvp6134 {

class Board;

namespace mpp::vi {

class InfoProvider : public Holder<Board*>,
        public hisilicon::mpp::vi::InfoProvider {
public:
    using Holder<Board*>::Holder;

private:
    bool configureImpl() override final;
};

}
}

#endif // BOARD_NVP6134_VI_INFO_PROVIDER_H
