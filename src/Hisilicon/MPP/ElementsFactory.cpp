#include "ElementsFactory.h"
#include "VideoBuffer.h"
#include "Vi/ViSubsystem.h"
#include "Vi/ViDevice.h"
#include "Vi/ViChannel.h"

namespace hisilicon {
namespace mpp {

ElementsFactory::ElementsFactory() {
}

ElementsFactory::~ElementsFactory() {
}

VideoBuffer *ElementsFactory::videoBuffer(MPP *p) {
    return new VideoBuffer(p);
}

ViSubsystem *ElementsFactory::vi(MPP *p) {
    return new ViSubsystem(p);
}

ViDevice *ElementsFactory::viDevice(MPP * p, int id) {
    return new ViDevice(p, id);
}

ViChannel *ElementsFactory::viChannel(MPP *m, ViDevice *d, int id) {
    return new ViChannel(m, d, id);
}

}
}
