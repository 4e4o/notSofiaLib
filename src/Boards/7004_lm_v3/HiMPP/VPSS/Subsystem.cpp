#include "Subsystem.h"

namespace boards::lm7004v3::vpss {

bool Subsystem::configureImpl() {
    // создаём группы и каналы по простой схеме
    addSourceFromVi1by1();
    return hisilicon::mpp::vpss::Subsystem::configureImpl();
}


}
