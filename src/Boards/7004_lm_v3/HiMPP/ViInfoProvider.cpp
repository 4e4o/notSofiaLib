#include "ViInfoProvider.h"

namespace boards::lm7004v3::mpp::vi {

// Для 7004_lm_v3 платы у нас 1 nvp чип
// и его каналы напрямую соединены с соотвествующими dev->way хики чипа
// Hi3520D／Hi3515A／Hi3515C H.264 CODEC Processor Data Sheet.pdf
// page 616

InfoProvider::TChannelId InfoProvider::inputIdToVi(const TChannelId &inputId) {
    if (inputId == TChannelId{0, 0}) {
        return {0, 0};
    } else if (inputId == TChannelId{0, 1}) {
        return {0, 1};
    } else if (inputId == TChannelId{0, 2}) {
        return {1, 0};
    } else if (inputId == TChannelId{0, 3}) {
        return {1, 1};
    }

    throw std::runtime_error("invalid nvp inputId.");
}

}
