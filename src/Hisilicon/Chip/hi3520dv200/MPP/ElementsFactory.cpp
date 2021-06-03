#include "ElementsFactory.h"
#include "Vi/ViChannel.h"
#include "Vi/ViSubsystem.h"
#include "Vi/ViDevice.h"
#include "MPP.h"

namespace hisilicon {
namespace mpp {
namespace hi3520dv200 {

ElementsFactory::ElementsFactory() {
}

ElementsFactory::~ElementsFactory() {
}

mpp::ViDevice* ElementsFactory::viDevice(mpp::MPP* p, int id) {
    return new ViDevice(p, id);
}

mpp::ViChannel *ElementsFactory::viChannel(mpp::MPP *m, mpp::ViDevice *d, int id) {
    return new ViChannel(m, d, id);
}

mpp::ViSubsystem *ElementsFactory::vi(mpp::MPP *p) {
    return new ViSubsystem(p);
}

}
}
}
