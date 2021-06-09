#include "MPP.h"
#include "VI/Channel.h"
#include "VI/Device.h"
#include "VI/Subsystem.h"

#define VEDU_COUNT 1

namespace hisilicon::mpp::hi3520dv200 {

MPP::MPP() {
    setVeduCount(VEDU_COUNT);

    registerType([](hisilicon::mpp::MPP* p) -> vi::Subsystem* {
        return new vi::hi3520dv200::Subsystem(p);
    });

    registerType([](vi::Subsystem* s, int id) -> vi::Device* {
        return new vi::hi3520dv200::Device(s, id);
    });

    registerType([](vi::Device* d, vi::ChannelInfo* i, int id) -> vi::Channel* {
        return new vi::hi3520dv200::Channel(d, i, id);
    });
}

}
