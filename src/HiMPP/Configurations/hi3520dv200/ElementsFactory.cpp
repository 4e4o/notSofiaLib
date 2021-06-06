#include "ElementsFactory.h"
#include "VI/Channel.h"
#include "VI/Subsystem.h"
#include "VI/Device.h"

namespace hisilicon::mpp::hi3520dv200 {

mpp::vi::Device* ElementsFactory::viDevice(mpp::vi::Subsystem* p, int id) {
    return new vi::hi3520dv200::Device(p, id);
}

mpp::vi::Channel *ElementsFactory::viChannel(mpp::vi::Device *d, mpp::vi::ChannelInfo* i, int id) {
    return new vi::hi3520dv200::Channel(d, i, id);
}

mpp::vi::Subsystem *ElementsFactory::vi(mpp::MPP *p) {
    return new vi::hi3520dv200::Subsystem(p);
}

}
