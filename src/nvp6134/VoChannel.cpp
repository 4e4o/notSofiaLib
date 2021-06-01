#include "VoChannel.h"
#include "DriverCommunicator.h"
#include "Chip.h"

#include <stdexcept>
#include <iostream>

namespace nvp6134 {

VoChannel::VoChannel(Chip* p, int id)
    : ChipChild(p), IdHolder(id) {
}

VoChannel::~VoChannel() {
}

bool VoChannel::setMode(unsigned char chid, NVP6134_OUTMODE_SEL mode) {
    return parent()->driver()->setVoChannelMode(this, chid, mode);
}

}
