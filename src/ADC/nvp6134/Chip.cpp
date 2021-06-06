#include "Chip.h"
#include "ViChannel.h"
#include "VoChannel.h"
#include "DriverCommunicator.h"

#include <stdexcept>

namespace nvp6134 {

Chip::Chip(DriverCommunicator* d, int id)
    : IdHolder(id),
      m_driver(d) {
}

Chip::~Chip() {
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

// Тут не всё понятно
// Во первых непонятна логика выбора, может она зависит от
// режим vo канала, может еще от чего.
// Определённо логика выбора зависит от формата сигнала на канале,
// от платы, от конфига железа.
// Класс конкретной конфигурации должен определять эту логику

NVP6134_VI_MODE Chip::getViChannelMode(ViChannel*) {
    throw std::runtime_error("Unimplemented getViChannelMode");
}

NVP6134_OUTMODE_SEL Chip::getVoChannelMode(VoChannel*) {
    throw std::runtime_error("Unimplemented getVoChannelMode");
}

bool Chip::configureImpl() {
    // создаём каналы
    for (int i = 0 ; i < (int) m_viChannels.size() ; i++)
        m_viChannels[i].reset(new ViChannel(this, i));

    for (int i = 0 ; i < (int) m_voChannels.size() ; i++)
        m_voChannels[i].reset(new VoChannel(this, i));

    // Устанавливаем режим Vi каналов
    for (auto& channel : m_viChannels)
        channel->setMode(getViChannelMode(channel.get()));

    // Устанавливаем режим Vo каналов
    for (int i = 0 ; i < (int) m_voChannels.size() ; i++)
        m_voChannels[i]->setMode(i, getVoChannelMode(m_voChannels[i].get()));

    // NOTE для NVP6134_OUTMODE_2MUX_MIX режима chid = номеру пары каналов
    // 0 - 0,1
    // !0 - 2,3
    // это в драйвере так сделано

    return true;
}

}
