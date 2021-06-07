#include "MPP.h"
#include "HiMPP/Configurations/hi3520dv200/VI/Channel.h"
#include "HiMPP/Configurations/hi3520dv200/VI/Device.h"
#include "HiMPP/Configurations/hi3520dv200/VI/Subsystem.h"

namespace hisilicon::mpp::lm7004v3 {

MPP::MPP() {
    registerType([](hisilicon::mpp::MPP* p) -> vi::Subsystem* {
        return new hisilicon::mpp::vi::hi3520dv200::Subsystem(p);
    });

    registerType([](vi::Subsystem* s, int id) -> vi::Device* {
        return new hisilicon::mpp::vi::hi3520dv200::Device(s, id);
    });

    registerType([](vi::Device* d, vi::ChannelInfo* i, int id) -> vi::Channel* {
        return new hisilicon::mpp::vi::hi3520dv200::Channel(d, i, id);
    });
}

void MPP::addSubsystems() {
    addViSubsystem();
    addVpssSubsystem();
}

}
