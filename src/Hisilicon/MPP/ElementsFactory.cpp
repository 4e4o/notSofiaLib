#include "ElementsFactory.h"
#include "VideoBuffer.h"
#include "VI/Subsystem.h"
#include "VI/Device.h"
#include "VI/Channel.h"

namespace hisilicon {
namespace mpp {

ElementsFactory::ElementsFactory() {
}

ElementsFactory::~ElementsFactory() {
}

VideoBuffer *ElementsFactory::videoBuffer(MPP *p) {
    return new VideoBuffer(p);
}

vi::Subsystem *ElementsFactory::vi(MPP *p) {
    return new vi::Subsystem(p);
}

vi::Device *ElementsFactory::viDevice(MPP * p, int id) {
    return new vi::Device(p, id);
}

vi::Channel *ElementsFactory::viChannel(MPP *m, vi::Device *d, int id) {
    return new vi::Channel(m, d, id);
}

}
}
