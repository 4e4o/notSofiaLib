#include "Chip.h"
#include "ViChannel.h"
#include "VoChannel.h"

namespace nvp6134 {

Chip::Chip(DriverCommunicator* d, int id)
    : IdHolder(id),
      m_driver(d) {
    init();
}

Chip::~Chip() {
}

void Chip::init() {
    for (int i = 0 ; i < (int) m_viChannels.size() ; i++)
        m_viChannels[i].reset(new ViChannel(this, i));

    for (int i = 0 ; i < (int) m_voChannels.size() ; i++)
        m_voChannels[i].reset(new VoChannel(this, i));
}

DriverCommunicator* Chip::driver() const {
    return m_driver;
}

Chip::TViChannels& Chip::viChannels() {
    return m_viChannels;
}

Chip::TVoChannels& Chip::voChannels() {
    return m_voChannels;
}

}
