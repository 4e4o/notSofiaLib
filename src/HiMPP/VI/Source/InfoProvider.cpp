#include "InfoProvider.h"
#include "ChannelInfo.h"

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

#ifdef FAKE_NOT_FOUNDED_CHANNEL_INFO
static ChannelInfo *fakeInfo() {
    ChannelInfo *info = new ChannelInfo();
    TSize size{.width = 352, .height = 240};

    info->setCapSize(size);
    info->setImgSize(size);

    info->setFps(25);
    info->setScanMode(VI_SCAN_PROGRESSIVE);
    info->setPixelFormat(PIXEL_FORMAT_YUV_SEMIPLANAR_422);

    return info;
}
#endif

const ChannelInfo *InfoProvider::getInfo(const TChannelId &viId) const {
    if (m_channels.contains(viId))
        return m_channels.at(viId);
    else {
#ifdef FAKE_NOT_FOUNDED_CHANNEL_INFO
        return fakeInfo();
#endif
    }

    return nullptr;
}

void InfoProvider::addChannel(const TChannelId &inputId,
                              const ChannelInfo *info) {
    const TChannelId viId = inputIdToVi(inputId);
    m_channels.emplace(std::move(viId), info);
}

}
