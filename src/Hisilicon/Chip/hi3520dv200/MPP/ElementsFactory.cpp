#include "ElementsFactory.h"
#include "VI/Channel.h"
#include "VI/Subsystem.h"
#include "VI/Device.h"

namespace hisilicon {
namespace mpp {
namespace hi3520dv200 {

mpp::vi::Device* ElementsFactory::viDevice(mpp::MPP* p, int id) {
    return new vi::hi3520dv200::Device(p, id);
}

mpp::vi::Channel *ElementsFactory::viChannel(mpp::MPP *m, mpp::vi::Device *d, int id) {
    return new vi::hi3520dv200::Channel(m, d, id);
}

mpp::vi::Subsystem *ElementsFactory::vi(mpp::MPP *p) {
    return new vi::hi3520dv200::Subsystem(p);
}

}
}
}
