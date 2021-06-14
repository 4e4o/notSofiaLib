#include "ConfiguratorBinder.h"
#include "HiMPP/VPSS/Binder/Binder.h"

namespace hisilicon::mpp::vpss {

void ConfiguratorBinder::bind(vpss::BindItem *s, vpss::BindItem *r) {
    addItemBack(new vpss::Binder(s, r));
}

}
