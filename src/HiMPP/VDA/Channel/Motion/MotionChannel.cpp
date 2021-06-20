#include "MotionChannel.h"
#include "MotionAttributes.h"
#include "MotionReader.h"

namespace hisilicon::mpp::vda {

MotionChannel::MotionChannel(Subsystem *s, int id)
    : Channel(new MotionAttributes(), new MotionReader(this), s, id) {
}

MotionChannel::~MotionChannel() {

}

MotionReader *MotionChannel::motionReader() const {
    return vdaReader<MotionReader>();
}

MotionAttributes *MotionChannel::attributes() const {
    return Channel::attributes<MotionAttributes>();
}

}
