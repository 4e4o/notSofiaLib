#include "ViChannel.h"
#include "DriverCommunicator.h"
#include "Chip.h"

#include <stdexcept>
#include <iostream>

namespace nvp6134 {

using namespace std;

ViChannel::ViChannel(Chip* p, int id)
    : ChipChild(p), IdHolder(id),
      m_videoFormat(TVideoFormat::DF_NOT_DETECTED) {
    init();
}

ViChannel::~ViChannel() {
}

void ViChannel::init() {
    DriverCommunicator::ViChannelFormat* fmt = parent()->driver()->getVideoFmt(this);

    if (fmt == NULL)
        throw std::runtime_error("Driver does not provide vi format");

    // TODO verify that getvideofmt exist in TDetectedFormat
    m_videoFormat = static_cast<TVideoFormat>(fmt->getvideofmt);
}

ViChannel::TVideoFormat ViChannel::videoFormat() const {
    return m_videoFormat;
}

bool ViChannel::pal() const{
    return (m_videoFormat == DF_CVBS_PAL) ||
            (m_videoFormat == DF_720P_PAL) ||
            (m_videoFormat == DF_720P_RT_PAL) ||
            (m_videoFormat == DF_1080P_PAL);
}

void ViChannel::setMode(NVP6134_VI_MODE m) {
    parent()->driver()->setViChannelMode(this, pal(), m);
}

}
