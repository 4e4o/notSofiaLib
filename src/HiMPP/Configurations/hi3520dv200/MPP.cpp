#include "MPP.h"
#include "VI/Subsystem.h"

#define VEDU_COUNT 1

namespace hisilicon::mpp::hi3520dv200 {

MPP::MPP() {
    setVeduCount(VEDU_COUNT);
    registerType<vi::Subsystem, vi::hi3520dv200::Subsystem, hisilicon::mpp::MPP *>();
}

}
