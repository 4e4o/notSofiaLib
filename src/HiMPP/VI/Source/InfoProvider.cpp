#include "InfoProvider.h"
#include "FakeChannelInfo.h"

// когда определён то на каждую не найденную инфу по каналу будет создан фейковый конфиг для захвата
// что позволит захватывать чёрные фреймы
//#define FAKE_NOT_FOUNDED_CHANNEL_INFO

namespace hisilicon::mpp::vi {

InfoProvider::InfoProvider() {
}

InfoProvider::~InfoProvider() {
    for (const auto & [key, value] : m_channels)
        delete value;

    m_channels.clear();
}

const IChannelInfo *InfoProvider::getInfo(const TChannelId &viId) const {
    if (m_channels.contains(viId))
        return m_channels.at(viId);
    else {
#ifdef FAKE_NOT_FOUNDED_CHANNEL_INFO
        return new FakeChannelInfo();
#endif
    }

    return nullptr;
}

void InfoProvider::addChannel(const TChannelId &inputId,
                              const IChannelInfo *info) {
    const TChannelId viId = inputIdToVi(inputId);
    m_channels.emplace(std::move(viId), info);
}

}
