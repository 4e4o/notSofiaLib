#include "Subsystem.h"

namespace boards::lm7004v3::vpss {

bool Subsystem::configureImpl() {
    if (!hisilicon::mpp::vpss::Subsystem::configureImpl())
        return false;

    addSourceFromVi1by1();

    return hisilicon::mpp::vpss::Subsystem::configureImpl();
}


}
