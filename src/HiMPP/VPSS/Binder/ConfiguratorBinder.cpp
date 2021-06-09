#include "ConfiguratorBinder.h"
#include "HiMPP/VPSS/Binder/Binder.h"

namespace hisilicon::mpp::vpss {

void ConfiguratorBinder::bind(vpss::BindSource* s, vpss::BindReceiver* r) {
    addItem(new vpss::Binder(s, r));
}

}
