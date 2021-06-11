#include "Subsystem.h"

namespace boards::lm7004v3::venc {

bool Subsystem::configureImpl() {
    // создаём группы и каналы по простой схеме
    addSourceFromVpss1by1();
    return hisilicon::mpp::venc::Subsystem::configureImpl();
}

}
