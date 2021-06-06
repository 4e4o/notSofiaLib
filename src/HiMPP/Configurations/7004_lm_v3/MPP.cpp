#include "MPP.h"
#include "HiMPP/ElementsFactory.h"

namespace hisilicon::mpp::lm7004v3 {

MPP::MPP(ElementsFactory* f)
    : mpp::MPP(f) {
    addViSubsystem();
    addVpssSubsystem();
}

}
