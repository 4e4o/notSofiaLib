#include "ElementsFactory.h"
#include "VideoBuffer.h"
#include "VI/Subsystem.h"
#include "VI/Device.h"
#include "VI/Channel.h"
#include "VPSS/Channel.h"
#include "VPSS/Group.h"
#include "VPSS/Subsystem.h"

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

vi::Device *ElementsFactory::viDevice(vi::Subsystem* p, int id) {
    return new vi::Device(p, id);
}

vi::Channel *ElementsFactory::viChannel(vi::Device *d, vi::ChannelInfo* i, int id) {
    return new vi::Channel(d, i, id);
}

vpss::Subsystem* ElementsFactory::vpss(MPP* p) {
    return new vpss::Subsystem(p);
}

vpss::Group* ElementsFactory::vpssGroup(vpss::Subsystem* p, int id) {
    return new vpss::Group(p, id);
}

vpss::Channel* ElementsFactory::vpssChannel(vpss::Group* g, int id) {
    return new vpss::Channel(g, id);
}

}
}
